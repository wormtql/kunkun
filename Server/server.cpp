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
int groupnum = 0;

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
        return true;
    }
    return false;
}

bool check_password( const json recv_msg )
{
    //get username
    string username = recv_msg["username"];
    string user_password = recv_msg["password"];

    //check_password with mysql ... mzh

}

bool check_username_exist( const string username )
{
    //check_username_exist with mysql ... mzh
}

bool create_group( const int group_id, const json recv_msg )
{
    string group_name = recv_msg["group_name"];
    string group_creater = recv_msg["who"];

    //create_group with mysql ... mzh

}

bool rename_group( const json recv_msg )
{
    int group_id = recv_msg["group_id"];
    string new_name = recv_msg["new_name"];

    //rename_group with mysql ... mzh

}

bool send_add_friend_request( const json recv_msg )
{
    string to_name = recv_msg["to_name"];
    int to_fd;
    if( to_fd = username_to_fd["to_name"] )
    { // onlion operation
        string s = recv_msg.dump();
        char *c = s.data();
        int ret = send(fd, c, sizeof(c), 0);
        if( !ret ){
            printf("send add friend request fail");
            return 0;
        }
    }
    else
    {
        // off-line operation
    }
    return 1;
}

json init_return_json( const json recv_msg )
{
    json j;
    j["debug"] = true; // 调试时修改
    j["command"] = recv_msg["command"];
    j["status"] = flase;
    j["msg"] = "";
    return j;
}

// bool send_file( const string file_name, const int fd )
// {
//     string file_url_s = "files/" + filename;
//     char *file_url_c = file_url_s.data();
//     FILE *fp = fopen(file_url_c, "wb");
//     if( fp == NULL )
//     {
//         printf("Cannot open file\n");
//         return 0;
//     }
//     char buffer[BUFSIZE] = {0};
//     int recv_len;
//     while( 1 ){
//         recv_len = recv(sock, buffer, BUFSIZE, 0);
//         json j = json::parse( )
//         if()
//         fwrite(buffer, recv_len, 1, fp);
//     }
//     fclose(fp);
// }
// void process_file( const json recv_msg, const int fd )
// {
//     json return_msg = init_return_json( recv_msg );
//     if( recv_msg["command"] == "chat_send_file")
//     {
//         bool to_exist = check_username_exist( recv_msg["to"] );
//         if( to_exist )
//         {
//             return_msg["status"] = true;
//             process_file( recv_msg );
//         }
//         else
//         {
//            return_msg["msg"] = "user " + recv_msg["to"] + " doesnt exist"; 
//         }
//         string s = return_msg.dump();
//         char *buf = s.data();
//         int ret = send(fd, buf, sizeof(buf), 0);
//         if (ret == -1) {
//             printf("send fail\n");
//         } else {
//             printf("send complete\n");
//         }
//         bool send_file_res = send_file( recv_msg["file_name"], sd );
//     }
// }

void process_msg( const json recv_msg, const int fd )
{
    json return_msg = init_return_json( recv_msg );
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
                printf("%s log in\n", username.data());
            }
            else
            {
                return_msg["msg"] = "password incorrect";
            }
        }
    }
    else if( recv_msg["command"] == "signup" )
    {
        bool username_exist = check_username_exist( recv_msg["username"] );
        if( username_exist )
        {
            return_msg["msg"] = recv_msg["username"] + " has already exist";
        }
        else
        {
            string username = recv_msg["username"];
            username_to_fd[username] = fd;
            fd_to_username[fd] = username;
            return_msg["status"] = true;
            printf("%s sign up\n", username.data());
        }
    }
    else if( recv_msg["command"] == "create_group" )
    {
        bool creat_group_res = creat_group( ++groupnum, recv_msg );
        if( creat_group_res ){
            return_msg["status"] = true;
            return_msg["group_id"] = groupnum;
        }else{
            return_msg["msg"] = "error with mysql";
        }
    }
    else if( recv_msg["command"] == "rename_group" )
    {
        bool rename_group_res = rename_group( recv_msg );
        if( rename_group_res ){
            return_msg["status"] = true;
        }else{
            return_msg["msg"] = "error with mysql";
        }
    }
    else if( recv_msg["command"] == "send_add_friend_request" )
    {
        bool username_exist = check_username_exist( recv_msg["to"] );
        if( !username_exist ){
            return_msg["msg"] = recv_msg["to"] + " doesnt exist";
        }else{
            bool send_add_friend_request_res = send_add_friend_request( recv_msg );
            if( send_add_friend_request_res )
            {
                return_msg["status"] = true;
            }
            else
            {
                return_msg["msg"] = "send request failed";
            }
            
        }
    }
    else if( recv_msg["command"] == "chat_send_file_begin" )
    {

    }

    // send massage to client
    string s = return_msg.dump();
    char *c = s.data();
    int ret = send(fd, c, sizeof(c), 0);
    if (ret == -1) {
        printf("send fail\n");
    } else {
        printf("send complete\n");
    }
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
