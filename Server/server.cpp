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
#include <mysql.h>
#include "json.hpp"

// #include "json.hpp"

#define myport 1234
#define BUFSIZE 20000
using json = nlohmann::json;
using std::map;
using std::string;
using std::vector;

// recv buffer
char buf[BUFSIZE] = { 0 };

// socket id to username
map<int, string> fd_to_username;
// username to socket id
map<string, int> username_to_fd;
// alive socket
vector<int> alive_socket;
int lock = 0;

// logout and map_erase
void check_logout( const int fd ){
    string username = fd_to_username[fd];
    username_to_fd.erase( username );
    fd_to_username.erase( fd );
}

// check user login
bool check_login( const json recv_msg )
{
    //get username
    string username = recv_msg["username"];
    // check login with map
    if( username_to_fd.find( username ) != username_to_fd.end() )
    {
        return 1;
    }
    return 0;
}

bool check_password( const json recv_msg )
{
    //get username
    string username = recv_msg["username"];
    string user_password = recv_msg["password"];

    //check_password with mysql ... mzh

}

json init_return_json( json recv_msg )
{
    json j;
    j["debug"] = true; // 调试时修改
    j["command"] = recv_msg["command"];
    j["status"] = flase;
    j["msg"] = "";
    return j;
}


void process_msg( json recv_msg, int fd )
{
    json return_msg = init_return_json( recv_msg );
    char buf[BUFSIZE] = {0};
    if( recv_msg["command"] == "login" ) // login
    { 
        bool already_login = check_login( recv_msg );
        if( already_login )
        {
            return_msg["msg"] = recv_msg["username"] + " had log in";
        }
        else
        {
            bool password_match = check_password( recv_msg );
            if( password_match )
            {
                string username = recv_msg["username"];
                username_to_fd[username] = fd;
                fd_to_username[fd] = username;
                return_msg["status"] = true;
            }
            else
            {
                return_msg["msg"] = "password incorrect";
            }
        }
    }
    else if( recv_msg["command"] == "signup" )
    {

    }

    string 
    //

}

const char debug_str[100] = "{\"debug\":true}";

// user functional thread function
void * thread_func(void * data) {
    printf("thread_started\n");

    while (1) {
        // require lock;
        while (lock == 1);
        lock = 1;

        auto iter = alive_socket.begin();
        while (iter != alive_socket.end()) {
            int fd = *iter;

            int status = recv(fd, buf, 20000, 0);

            if (status == 0) {
                printf("user %d exited\n", fd);
                // user log out
                check_logout( fd );
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
                json j = json::parse(buf);
                printf("recv from socket %d: %s\n", fd, buf);
                //process_msg( j, fd );

                int ret = send(fd, debug_str, sizeof(debug_str), 0);
                if (ret == -1) {
                    printf("send fail\n");
                } else {
                    printf("send complete\n");
                }
            }
            
            iter++;
        }

        lock = 0;
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

            while (lock == 1);
            lock = 1;
            alive_socket.push_back(new_socketfd);
            lock = 0;
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
