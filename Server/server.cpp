#include <map>
#include <string>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <gtk/gtk.h>
#include <fcntl.h>

// #include "json.hpp"

#define myport 1234

using std::map;
using std::string;
using std::vector;

// recv buffer
char buf[20000] = { 0 };

// socket id to username
map<int, string> fd_to_username;
// username to socket id
map<string, int> username_to_fd;
// alive socket
vector<int> alive_socket;
GMutex mutex;


const char debug_str[100] = "{\"debug\":true}";


// user functional thread function
void * thread_func(void * data) {
    printf("thread_started\n");

    while (1) {
        // require lock;
        g_mutex_lock(&mutex);

        auto iter = alive_socket.begin();
        while (iter != alive_socket.end()) {
            int fd = *iter;

            memset(buf, 0, sizeof(buf));
            int status = recv(fd, buf, 20000, 0);

            if (status == 0) {
                printf("user %d exited\n", fd);
                
                iter = alive_socket.erase(iter);
                continue;
            } else if (status == -1) {
                if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
                    // nothing
                } else {
                    // error
                    printf("error in socket %d\n", fd);
                }
                
            } else {
                // handle msg here
                printf("recv from socket %d: %s\n", fd, buf);

                int ret = send(fd, debug_str, sizeof(debug_str), 0);
                if (ret == -1) {
                    printf("send fail\n");
                } else {
                    printf("send complete\n");
                }
            }
            
            iter++;
        }

        g_mutex_unlock(&mutex);
    }
}


int main(int argc, char * argv[]) {
    int socketfd, new_socketfd;
    sockaddr_in sin_addr, pin_addr;
    unsigned int len, pin_addr_size, i;
    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("cant create socket\n");
        exit(1);
    }

    memset(&sin_addr, 0, sizeof(sin_addr));
    sin_addr.sin_family = AF_INET;
    // sin_addr.sin_addr.s_addr = htonl("127.0.0.1");
    sin_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sin_addr.sin_port = htons(myport);
    if (bind(socketfd, (sockaddr *)&sin_addr, sizeof(sin_addr)) < 0) {
        printf("cant bind socket\n");
        exit(1);
    }

    if (listen(socketfd, 5) < 0) {
        printf("listen error\n");
        exit(1);
    }

    g_thread_new("thread2", thread_func, nullptr);

    printf("start accepting\n");

    while (1) {
        // printf("aaa\n");
        if ((new_socketfd = accept(socketfd, (sockaddr *)&pin_addr, &pin_addr_size)) < 0) {
            printf("accept error\n");
            exit(1);
        } else {
            printf("socket accepted\n");

            int flags = fcntl(new_socketfd, F_GETFL, 0);
            fcntl(new_socketfd, F_SETFL, flags | O_NONBLOCK);

            // add alive socket

            // while (lock == 1);
            g_mutex_lock(&mutex);
            alive_socket.push_back(new_socketfd);
            g_mutex_unlock(&mutex);
            // lock = 0;
	    }

        // if (recv(new_socketfd, buf, 200, 0) == -1) {
        //     printf("cant receive packet\n");
        //     exit(1);
        // }
        // printf("received from client: %s\n", buf);

        // len = strlen(buf);
        // for (i = 0; i < len; i++) {
        //     buf[i] = toupper(buf[i]);
        // }
        // if (send(new_socketfd, buf, len, 0) == -1) {
        //     printf("cant send packet\n");
        //     exit(1);
        // }

        // close(new_socketfd);
    }

    return 0;
}
