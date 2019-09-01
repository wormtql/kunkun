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
#include <mysql/mysql.h>
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

struct fp_fd{
    string filename;
    int fileid;
    FILE *fp;
    int fd;
    fp_fd(){}
    fp_fd( string filename, int fileid, FILE *fp, int fd ) : filename(filename), fileid(fileid), fp(fp), fd(fd) {}
};
vector<fp_fd> alive_file;
//int lock = 0;

GMutex alive_socket_mutex;
int groupnum = 0;
int filenum = 0;


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
    //get username and password
    string username = recv_msg["username"];
    string user_password = recv_msg["password"];

    //check_password with mysql ... mzh


    return true;
}

bool check_username_exist( const string username )
{
    //check_username_exist with mysql ... mzh

    return true;
}

bool create_group( const int group_id, const json recv_msg )
{
    string group_name = recv_msg["group_name"];
    string group_creater = recv_msg["who"];

    //create_group with mysql ... mzh


    return true;
}

bool rename_group( const json recv_msg )
{
    int group_id = recv_msg["group_id"];
    string new_name = recv_msg["new_name"];

    //rename_group with mysql ... mzh
    return true;
}


// 向被请求者发送请求消息
bool send_add_friend_request( const json recv_msg )
{
    string to_name = recv_msg["to"];
    json to_msg;
    to_msg["debug"] = true;
    to_msg["command"] = "recv_add_friend_request";
    to_msg["from"] = recv_msg["from"];
    to_msg["to"] = recv_msg["to"];
    if (username_to_fd.find(to_name) != username_to_fd.end())
    { // online operation

        // 如果对方在线就发给对方

        string s = to_msg.dump();
        char *c = s.data();
        int ret = send(fd, c, strlen(c), 0);


        int fd = username_to_fd[to_name];

        int ret = send(fd, s.c_str(), sizeof(char) * s.size(), 0);
        if( !ret ){
            printf("send add friend request fail\n");
            return 0;
        }
    }

    // todo 数据库中的请求表更新

    return 1;
}


// 向请求者发送被请求者的 接受/不接受 信息
bool send_add_friend_result( const json recv_msg )
{
    string to_name = recv_msg["from"];
    json to_msg;
    to_msg["debug"] = true;
    to_msg["command"] = "recv_add_friend_result";
    to_msg["from"] = recv_msg["from"];
    to_msg["to"] = recv_msg["to"];
    to_msg["accept"] = recv_msg["accept"];

    if (username_to_fd.find(to_name) != username_to_fd.end())
    { // online operation
        string s = to_msg.dump();
        char *c = s.data();
        int ret = send(fd, c, strlen(c), 0);

        int fd = username_to_fd[to_name];

        int ret = send(fd, s.c_str(), sizeof(char) * s.size(), 0);
        if( !ret ){
            printf("send add friend request fail\n");
            return 0;
        }
    }
    else
    {
        // todo off-line operation
    }

    if( recv_msg["accept"] == true )
    {
        // todo update friend list with mysql ... mzh
    }

    return 1;
}

bool chat_send_msg( const json recv_msg )
{
    string to_name = recv_msg["to"];
    json to_msg;
    to_msg["debug"] = true;
    to_msg["command"] = "chat_recv_msg";
    to_msg["from"] = recv_msg["from"];
    to_msg["to"] = recv_msg["to"];
    to_msg["content"] = recv_msg["content"];
    if( username_to_fd.find(to_name) != username_to_fd.end() )
    { // onlion operation
        string s = to_msg.dump();
        char *c = s.data();
        int ret = send(fd, c, strlen(c), 0);

        int fd = username_to_fd[to_name];

        int ret = send(fd, s.c_str(), sizeof(char) * s.size(), 0);

        if( !ret ){
            printf("send massage from %s to %s fail\n", ((string)recv_msg["from"]).c_str(), to_name.data() );
            return 0;
        }
    }
    else
    {
        // off-line operation
    }

    // update chat history with mysql ... mzh

    return 1;
}

bool group_send_msg( const json recv_msg )
{
    string name = recv_msg["username"];
    int group_id = recv_msg["group_id"];

    // todo broadcast message to group members
    // todo update group history with mysql ... mzh

    return 1;
}


json init_return_json( const json recv_msg )
{
    json j;
    j["debug"] = true; // remember to change with debug
    j["command"] = recv_msg["command"];
    j["status"] = false;
    j["msg"] = "";
    return j;
}

bool recv_file( const int fileid, const int fd )
{
    // require a lock

    char file_url[50] = {0};
    string filename = find_filename( fileid, file_url ); // with mysql ... mzh
    FILE *fp = fopen( file_url, "rb" );
    if(fp == NULL){
        return 0;
    }
    fp_fd new_file( filename, fileid, fp, fd );
    alive_file.insert( new_file );

    // require a lock
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

    string cmd = recv_msg["command"];


    if( recv_msg["command"] == "login" ) // login
    {
        bool already_login = check_login( recv_msg );
        if( already_login )
        {
            return_msg["msg"] = (string)recv_msg["username"] + " had log in";
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
            return_msg["msg"] = (string)recv_msg["username"] + " has already exist";
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
        bool creat_group_res = create_group( ++groupnum, recv_msg );
        if( creat_group_res )
        {
            return_msg["status"] = true;
            return_msg["group_id"] = groupnum;
        }
        else
        {
            return_msg["msg"] = "error with mysql";
        }
    }
    else if( recv_msg["command"] == "rename_group" )
    {
        bool rename_group_res = rename_group( recv_msg );
        if( rename_group_res )
        {
            return_msg["status"] = true;
        }
        else
        {
            return_msg["msg"] = "error with mysql";
        }
    }
    else if( recv_msg["command"] == "send_add_friend_request" )
    {
        bool username_exist = check_username_exist( recv_msg["to"] );
        if( !username_exist )
        {
            return_msg["msg"] = (string)recv_msg["to"] + " doesnt exist";
        }
        else
        {
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
    else if( recv_msg["command"] == "send_add_friend_result" )
    {
        bool send_add_friend_result_res = send_add_friend_result( recv_msg );
        if( send_add_friend_result_res )
        {
            return_msg["status"] = true;
        }
        else
        {
            return_msg["msg"] = "send result failed";
        }
    }
    else if( recv_msg["command"] == "chat_send_msg" )
    {
        bool username_exist = check_username_exist( recv_msg["to"] );
        if( !username_exist )
        {
            return_msg["msg"] = (string)recv_msg["to"] + " doesnt exist";
        }
        else
        {
            bool chat_send_msg_res = chat_send_msg( recv_msg );
            if( chat_send_msg_res )
            {
                return_msg["status"] = true;
            }
            else
            {

            }
        }
    }
    else if( recv_msg["command"] == "group_send_msg" )
    {
        bool group_send_msg_res = group_send_msg( recv_msg );
        if( group_send_msg_res )
        {
            return_msg["status"] = true;
        }
        else
        {

        }
    }
    else if( recv_msg["command"] == "chat_send_file_begin" )
    {
        bool file_create_res = file_create( recv_msg["filename"] );
        if( !file_create_res )
        {
            return_msg["msg"] = "fail to send file, try it later";
        }
        else
        {

        }
    }
    else if( recv_msg["command"] == "chat_send_file" )
    {

    }
    else if( recv_msg["command"] == "send_me_a_file" ) {
        recv_file(recv_msg["fileid"]);
    }
    else if ( cmd == "send_join_group_request" )
    {

    }
    else if (cmd == "alter_user_info")
    {

    }
    else if (cmd == "get_user_info")
    {

    }
    else if (cmd == "list_user_like")
    {

    }
    else if (cmd == "list_group_like")
    {

    }
    else if (cmd == "chat_send_file")
    {

    }

    // send massage to client
    string s = return_msg.dump();
    char *c = s.data();
    int ret = send(fd, c, strlen(c), 0);
    int ret = send(fd, s.c_str(), sizeof(char) * s.size(), 0);
    if (ret == -1) {
        printf("send fail\n");
    } else {
        printf("send complete\n");
    }
    //

}

const char debug_str[100] = "{\"debug\":true}";

// process file thread function
void * file_thread_func( void *data )
{
    char buffer[1024];
    int buffer_size;
    json send_msg;
    send_msg["command"] = "recv_file";
    printf("file_thread_started\n");
    while (1) {
        // require lock;

        auto iter = alive_file.begin();
        while( iter != alive_file.end() )
        {
            int fd = (*iter).fd;
            FILE *fp = (*iter).fp;
            send_msg["filename"] = (*iter).filename;
            send_msg["fileid"] = (*iter).fileid;
            buffer_size = fread(buffer, 1, BUF_SIZE, fp);
            if( buffer_size == 0 )
            {
                send_msg["eof"] = true;
                string s = send_msg.dump();
                char *c = s.data;
                send( fd, c, strlen(c), 0 );
                fclose( fp );
                alive_file.erase( iter );
                continue;
            }
            else
            {
                send_msg["eof"] = false;
                string s = buffer;
                send_msg["content"] = s;
                send( fd, buffer, buffer_size, 0);
            }
            iter ++;
        }

        // require lock

    }
}

// user functional thread function
void * thread_func(void * data) {
    printf("thread_started\n");

    while (1) {
        // require lock;
        g_mutex_lock(&alive_socket_mutex);

        auto iter = alive_socket.begin();
        while (iter != alive_socket.end()) {
            int fd = *iter;

            memset(buf, 0, sizeof(buf));
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

                int ret = send(fd, debug_str, strlen(debug_str), 0);
                if (ret == -1) {
                    printf("send fail\n");
                } else {
                    printf("send complete\n");
                }
            }

            iter++;
        }

        g_mutex_unlock(&alive_socket_mutex);
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
    g_thread_new("thread3", file_thread_func, nullptr);
    
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
            g_mutex_lock(&alive_socket_mutex);
            alive_socket.push_back(new_socketfd);
            g_mutex_unlock(&alive_socket_mutex);
	    }
    }

    return 0;
}
