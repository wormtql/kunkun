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
// socket to mutex
map<SOCKET, GMutex> fd_to_mutex;
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

int mysend( SOCKET s, const char FAR *buf, int len, int flags )
{
    GMutex socket_mutex = fd_to_mutex(s);
    g_mutex_lock(&socket_mutex);
    int ret = send( s, buf, len, flags );
    g_mutex_unlock(&socket_mutex);
    return ret;
}
// logout and map_erase
void check_logout( const int fd )
{
    string username = fd_to_username[fd];
    username_to_fd.erase( username );
    fd_to_username.erase( fd );
    fd_to_gmutex.erase( fd );
}

// 在线检测
bool check_login( const json recv_msg, string &msg )
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

// 登录
bool check_password( const json recv_msg, string &msg )
{
    //get username and password
    string username = recv_msg["username"];
    string user_password = recv_msg["password"];
    bool res = _login( username, user_password, msg ); // todo mysql
    return res;
}

// 注册
bool signup( const json recv_msg, string &msg )
{
    username = recv_msg["username"];
    password = recv_msg["password"];
    bool res = _signup( username, password, msg ); // todo mysql
    return res;
}

// 创建群聊
bool create_group( const json recv_msg, int &groupid, string &msg )
{
    string groupname = recv_msg["group_name"];
    string groupcreater = recv_msg["who"];
    bool res = _create_group( groupname, groupcreater, groupid, msg ); // todo mysql
    return res;
}

// 重命名群聊
bool rename_group( const json recv_msg )
{
    int groupid = recv_msg["group_id"];
    string new_name = recv_msg["new_name"];
    string username = recv_msg["who"];
    bool res = _rename_group( groupid, username, new_name, string &msg ); // todo mysql
    return res;
}

// 向被请求者发送请求消息
bool send_add_friend_request( const json recv_msg )
{
    string to_name = recv_msg["to"];
    string from_name = recv_msg["from"];
    json to_msg;
    to_msg["debug"] = true;
    to_msg["command"] = "recv_system_msg";
    to_msg["msg"] = "You get a request of adding friend from " + from_name;
    if (username_to_fd.find(to_name) != username_to_fd.end())
    { // online operation

        // 如果对方在线就发给对方

        string s = to_msg.dump();
        char *c = s.data();
        int ret = send(fd, c, strlen(c), 0);

        int fd = username_to_fd[to_name];

        int ret = send(fd, s.c_str(), sizeof(char) * s.size(), 0);
        if( !ret ){
            printf("send system massage fail\n");
            return 0;
        }
    }

    // todo 数据库中的请求表更新

    return 1;
}

bool send_join_group_request( const json recv_msg )
{
    string username = recv_msg["from"];
    int groupid = recv_msg["group_id"];
    string groupname = find_groupname( groupid ); // with mysql ... mzh
    string to_name = find_groupcreater( groupid ); // with mysql ... mzh
    json to_msg;
    to_msg["debug"] = true;
    to_msg["command"] = "recv_system_msg";

    to_msg["msg"] = "You get a request of jioning group " + groupname + " from " + username;
    if (username_to_fd.find(to_name) != username_to_fd.end())
    { // online operation

        // 如果对方在线就发给对方

        string s = to_msg.dump();
        char *c = s.data();
        int ret = send(fd, c, strlen(c), 0);

        int fd = username_to_fd[to_name];

        int ret = send(fd, s.c_str(), sizeof(char) * s.size(), 0);
        if( !ret ){
            printf("send system massage fail\n");
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
    string from_name = recv_msg["to"];
    bool accept = recv_msg["accept"];
    json to_msg;
    to_msg["debug"] = true;
    to_msg["command"] = "recv_system_msg";
    if( accept ) to_msg["msg"] = from_name + " accept your friend request";
    else to_msg["msg"] = from_name + " refuse your friend request";

    if (username_to_fd.find(to_name) != username_to_fd.end())
    { // online operation
        string s = to_msg.dump();
        char *c = s.data();
        int ret = send(fd, c, strlen(c), 0);
        int fd = username_to_fd[to_name];

        int ret = send(fd, s.c_str(), sizeof(char) * s.size(), 0);
        if( !ret ){
            printf("send system massage fail\n");
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

bool send_join_group_result( const json recv_msg )
{
    string to_name = recv_msg["who"];
    string groupname = find_groupname( groupid ); // with mysql ... mzh
    bool accept = recv_msg["accept"];
    json to_msg;
    to_msg["debug"] = true;
    to_msg["command"] = "recv_system_msg";
    if( accept ) to_msg["msg"] = groupname + " accept your join_group request";
    else to_msg["msg"] = groupname + " refuse your join_group request";

    if (username_to_fd.find(to_name) != username_to_fd.end())
    { // online operation
        string s = to_msg.dump();
        char *c = s.data();
        int ret = send(fd, c, strlen(c), 0);
        int fd = username_to_fd[to_name];

        int ret = send(fd, s.c_str(), sizeof(char) * s.size(), 0);
        if( !ret ){
            printf("send system massage fail\n");
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

bool alter_user_info( const json recv_msg )
{
    // todo with mysql ... mzh
}

bool get_user_info( const json recv_msg )
{
    // todo with mysql ... mzh
}

bool list_user_like( const json recv_msg )
{
    // todo with mysql ... mzh
}

bool list_group_like( const json recv_msg )
{
    // todo with mysql ... mzh
}

bool chat_send_msg( const json recv_msg )
{
    string to_name = recv_msg["to"];
    json to_msg;
    json text;
    text["type"] = "text";
    text["content"] = recv_msg["content"];
    to_msg["debug"] = true;
    to_msg["command"] = "chat_recv_msg";
    to_msg["from"] = recv_msg["from"];
    to_msg["to"] = recv_msg["to"];
    to_msg["content"] = text;
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
    vector<string>group_member = // to with mysql
    json to_msg;
    json text;
    text["type"] = "text";
    text["content"] = recv_msg["content"];
    to_msg["debug"] = true;
    to_msg["command"] = "group_recv_msg";
    to_msg["from"] = recv_msg["from"];
    to_msg["content"] = text;
    auto iter = group_member.begin();
    while( iter != group_member.end() )
    {
        to_name = *iter;
        if( username_to_fd.find(to_name) != username_to_fd.end() )
        { // onlion operation
            string s = to_msg.dump();
            char *c = s.data();
            int ret = send(fd, c, strlen(c), 0);

            int fd = username_to_fd[to_name];

            int ret = send(fd, s.c_str(), sizeof(char) * s.size(), 0);
            
        }
    }
    // todo broadcast message to group members
    // todo update group history with mysql ... mzh

    return 1;
}

bool list_friend_request( const json recv_msg, json &return_msg )
{
    string username = recv_msg["username"];
    vector<string>list = // ... with mysql 
    json j(list);
    return_msg["list"] = j;
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

void process_msg( const json recv_msg, const int fd )
{
    json return_msg = init_return_json( recv_msg );
    string msg;
    string cmd = recv_msg["command"];

    if( cmd == "login" ) // login
    {
        bool already_login = check_login( recv_msg );
        if( already_login )
        {
            return_msg["msg"] = (string)recv_msg["username"] + " had log in";
        }
        else
        {
            bool password_match = check_password( recv_msg, msg );
            if( password_match )
            {
                string username = recv_msg["username"];
                // add online_map
                username_to_fd[username] = fd;
                fd_to_username[fd] = username;
                GMutex newmutex;
                fd_to_gmutex[fd] = newmutex;

                return_msg["status"] = true;
                printf("%s login\n", username.data());
            }
            else
            {
                return_msg["msg"] = msg;
            }
        }
    }
    else if( cmd == "signup" )
    {
        bool signup_res = signup( recv_msg, msg );
        if( username_exist )
        {
            return_msg["msg"] = msg;
        }
        else
        {
            string username = recv_msg["username"];
            // add online_map
            username_to_fd[username] = fd;
            fd_to_username[fd] = username;
            GMutex newmutex;
            fd_to_gmutex[fd] = newmutex;

            return_msg["status"] = true;
            printf("%s signup and login\n", username.data());
        }
    }
    else if( cmd == "create_group" )
    {
    	int groupid;
        bool creat_group_res = create_group( recv_msg, groupid, msg );
        if( creat_group_res )
        {
            return_msg["status"] = true;
            return_msg["group_id"] = groupid;
        }
        else
        {
            return_msg["msg"] = msg;
        }
    }
    else if( cmd == "rename_group" )
    {
        bool rename_group_res = rename_group( recv_msg, msg );
        if( rename_group_res )
        {
            return_msg["status"] = true;
        }
        else
        {
            return_msg["msg"] = msg;
        }
    }
    else if( cmd == "send_add_friend_request" )
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
    else if( cmd == "send_join_group_request" )
    {
        bool username_exist = check_username_exist( recv_msg["to"] );
        if( !username_exist )
        {
            return_msg["msg"] = (string)recv_msg["to"] + " doesnt exist";
        }
        else
        {
            bool send_join_group_res = send_join_group_request( recv_msg );
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
    else if( cmd == "chat_send_msg" )
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
    else if( cmd == "group_send_msg" )
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
    else if( cmd == "chat_send_file_begin" )
    {
        bool file_create_res = file_create( recv_msg["filename"] );
        if( !file_create_res )
        {
            return_msg["msg"] = "fail to send file, try it later";
        }
        else
        {
        	return_msg[""]
        }
    }
    else if( cmd == "chat_send_file" )
    {
    }
    else if( cmd == "send_me_a_file" ) {
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
