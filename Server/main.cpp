#include <map>
#include <string>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <iostream>
//#include <direct.h>
#include <dirent.h>
#include <sys/stat.h>
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

#include "Database.h"

// #include "json.hpp"

#define myport 1234
#define BUFSIZE 20000

#define SOCKET int

//using json = nlohmann::json;
//using std::map;
//using std::string;
//using std::vector;
//using std::ofstream;
//using std::ifstream;

using namespace std;

// recv buffer
char buf[BUFSIZE] = { 0 };

// socket id to username
map<int, string> fd_to_username;
// username to socket id
map<string, int> username_to_fd;
// socket to mutex
//map<SOCKET, GMutex> fd_to_mutex;
map<SOCKET, GMutex *> fd_to_mutex;
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

// 创建文件夹（文件，聊天记录）
void new_floder()
{
    string folderPath = "files";
    if (access(folderPath.c_str(), 0) != 0 )
    {
//        mkdir(folderPath.c_str());   // 返回 0 表示创建成功，-1 表示失败
        mkdir(folderPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }
    folderPath = "chat";
    if (access(folderPath.c_str(), 0) != 0 )
    {
//        mkdir(folderPath.c_str());   // 返回 0 表示创建成功，-1 表示失败
        mkdir(folderPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }
    folderPath = "group";
    if (access(folderPath.c_str(), 0) != 0 )
    {
//        mkdir(folderPath.c_str());   // 返回 0 表示创建成功，-1 表示失败
        mkdir(folderPath.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }
}

// 加锁 send
int mysend( SOCKET s, const char *buf, int len, int flags )
{
    int ret = 0;
    if (fd_to_mutex.find(s) != fd_to_mutex.end()) {
        GMutex * socket_mutex = fd_to_mutex[s];
        g_mutex_lock(socket_mutex);
        ret = send( s, buf, len, flags );
        g_mutex_unlock(socket_mutex);
    } else {
        ret = send( s, buf, len, flags );
    }

    return ret;
}

// 系统发送信息
bool system_send( const string to_name, const string msg )
{
    json to_msg;
//    to_msg["debug"] = true;
    to_msg["debug"] = false;
    to_msg["command"] = "recv_sys_msg";
    to_msg["msg"] = msg;
    if ( username_to_fd.find(to_name) != username_to_fd.end() )
    {
        // 如果对方在线就发给对方
        string s = to_msg.dump();
        int fd = username_to_fd[to_name];
        int ret = mysend(fd, s.c_str(), s.size(), 0);
        // int ret = mysend(fd, s.c_str(), sizeof(char) * s.size(), 0);
        if( !ret ){
            printf("send system massage fail\n");
            return false;
        }
        return true;
    }
    else
    {
        return false;
    }
}

// 下线操作
void check_logout( const int fd )
{
    string username = fd_to_username[fd];
    username_to_fd.erase( username );
    fd_to_username.erase( fd );
    fd_to_mutex.erase( fd );
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
    string username = recv_msg["username"];
    string password = recv_msg["password"];
    bool res = _signup( username, password, msg ); // todo mysql
    return res;
}

// 创建群聊
bool create_group( const json recv_msg, int &groupid, string &msg )
{
    string groupname = recv_msg["group_name"];
    string groupcreater = recv_msg["who"];
    bool res = _create_group( groupname, groupcreater, groupid, msg ); // todo mysql
    if( res )
    {

        string path = "group/"+std::to_string( groupid );
        FILE *fp = fopen( path.c_str(), "wb");
        fclose( fp );
    }
    return res;
}

// 重命名群聊
bool rename_group( const json recv_msg, string & msg )
{
    int groupid = recv_msg["group_id"];
    string new_name = recv_msg["new_name"];
    string username = recv_msg["who"];
    bool res = _rename_group( groupid, username, new_name, msg ); // todo mysql
    return res;
}

// 发送好友请求
bool send_add_friend_request( const json recv_msg, string &msg )
{
    string to_name = recv_msg["to"];
    string from_name = recv_msg["from"];
    system_send( to_name, "You get a request of adding friend from " + from_name );
    // 数据库备案
    bool res = _send_add_friend_request( from_name, to_name, msg );
    return res;
}

// 发送群聊邀请
bool send_invite_to_group_request( const json recv_msg, string &msg )
{
    string from_name = recv_msg["from"];
    string to_name = recv_msg["to"];
    int groupid = recv_msg["group_id"];
    string groupname;
    bool res = _find_groupname( groupid, groupname, msg );
    if( !res ) return false;
    system_send( to_name, "You get a invitation of jioning group " + groupname + " from " + from_name );
    // 数据库备案
    res = _send_invite_to_group_request( from_name, to_name, groupid, msg );
    return res;
}

// 发送群聊申请
bool send_join_group_request( const json recv_msg, string &msg )
{
    string from_name = recv_msg["from"];
    int groupid = recv_msg["group_id"];
    string groupname, creater;
    bool res = _find_groupname( groupid, groupname, msg );
    if( !res ) return false;
    res = _find_group_creater( groupid, creater, msg );
    if( !res ) return false;
    system_send( creater, "You get a request of jioning group " + groupname + " from " + from_name );
    // 数据库备案
    res = _send_invite_to_group_request( from_name, creater, groupid, msg );
    return res;
}

// 修改用户信息
bool alter_user_info( const json recv_msg, string &msg )
{
    string username = recv_msg["who"];
    string field = recv_msg["field"];
    string new_val = recv_msg["new_val"];
    bool res = _alter_user_info( username, field, new_val, msg );
    return res;
}

// 查看用户信息
bool get_user_info( const json recv_msg, string &val, string &msg )
{
    string username = recv_msg["who"];
    string field = recv_msg["field"];
    bool res = _get_user_info( username, field, val, msg );
    return res;
}

// 查找用户
bool list_user_like( const json recv_msg, vector<string>&list, string &msg )
{
    string query_str = recv_msg["query_str"];
    bool res = _list_user_like( query_str, list, msg );
    return res;
}

// 查找群聊
bool list_group_like( const json recv_msg, vector<string>&list, string &msg )
{
    string query_str = recv_msg["query_str"];
    bool res = _list_group_like( query_str, list, msg );
    return res;
}

// 向请求者发送被请求者的 接受/不接受 信息
bool send_add_friend_result( const json recv_msg, string &msg )
{
    string to_name = recv_msg["from"];
    string from_name = recv_msg["to"];
    bool accept = recv_msg["accept"];
    if( accept )
    {
        system_send( to_name, from_name + " accept your friend request");
        bool res = _send_add_friend_result( to_name, from_name, msg );
        if( res )
        {
            // 创建聊天记录文件夹
            string path = "chat/"+to_name+"_"+from_name;
            FILE *fp = fopen( path.c_str(), "wb" );
            fclose( fp );
        }
        return res;
    }
    else
    {
        system_send( to_name, from_name + " refuse your friend request");
    }
    return true;
}

// 发送群聊邀请结果
bool send_invite_to_group_result( const json recv_msg, string &msg )
{
    string to_name = recv_msg["whom"];
    int groupid = recv_msg["group_id"];
    string groupname;
    bool res = _find_groupname( groupid, groupname, msg );
    if( !res ) return false;
    bool accept = recv_msg["accept"];
    if( accept )
    {
//        bool res = _send_invite_to_group_result( to_name, groupid, msg );
        bool res = _send_add_group_result(groupid, to_name, msg);
        return res;
    }
    return true;
}

// 发送群聊申请结果
bool send_join_group_result( const json recv_msg, string &msg )
{
    string to_name = recv_msg["who"];
    int groupid = recv_msg["group_id"];
    string groupname;
    bool res = _find_groupname( groupid, groupname, msg );
    if( !res ) return false;
    bool accept = recv_msg["accept"];
    if( accept )
    {
        system_send( to_name, "group " + groupname + " accept your join_group request");
        bool res = _send_add_group_result( groupid, to_name, msg );
        return res;
    }
    else
    {
        system_send( to_name, "group " + groupname + " refuse your join_group request");
    }
    return true;
}

// 聊天信息发送
bool chat_send_msg( const json recv_msg, string &msg )
{
    string to_name = recv_msg["to"];
    string from_name = recv_msg["from"];
    json to_msg;
    json text;
    text["type"] = "text";
    text["content"] = recv_msg["content"];
    to_msg["debug"] = false;
    to_msg["command"] = "chat_recv_msg";
    to_msg["from"] = from_name;
    to_msg["to"] = to_name;
    to_msg["content"] = text;
    json history;
    history["name"] = from_name;
    history["type"] = "text";
    history["content"] = recv_msg["content"];
    history["read"] = false;
    string path;
    bool res = _find_chat_path( to_name, from_name, path, msg );
    if( !res )
    {
        printf("find chat path of %s and %s failed\n", from_name.c_str(), to_name.data() );
        return false;
    }
    if( username_to_fd.find(to_name) != username_to_fd.end() )
    {
        // 如果对方在线
        string s = to_msg.dump();
        int fd = username_to_fd[to_name];
        int ret = mysend(fd, s.c_str(), s.size(), 0);
        if( !ret ){
            printf("send massage from %s to %s fail\n", from_name.c_str(), to_name.data() );
            return false;
        }
        history["read"] = true;
        ofstream out;
        out.open( path, std::ios::app );
        out << history.dump() << std::endl;
        out.close();
        return true;
    }
    else
    {
        // 如果对方离线
        // 更新历史记录，记录未读
        ofstream out;
        out.open( path, ios::app );
        out << history.dump() << endl;
        out.close();

        // 数据库记录未读信息
        bool res = _update_unread( from_name, to_name, msg );
        return res;
    }
}

// 群聊信息发送
bool group_send_msg( const json recv_msg, string & msg )
{
    string username = recv_msg["from"];
    int groupid = recv_msg["group_id"];
    vector<string>group_members;
    _get_group_members( groupid, group_members, msg );
    json to_msg;
    json text;
    text["type"] = "text";
    text["content"] = recv_msg["content"];
    to_msg["debug"] = false;
    to_msg["command"] = "group_recv_msg";
    to_msg["from"] = username;
    to_msg["content"] = text;
    json history;
    history["name"] = username;
    history["type"] = "text";
    history["content"] = recv_msg["content"];
    string path;
    bool res = _find_group_path( groupid, path, msg );
    if( !res )
    {
        printf("find group %d path failed\n", groupid );
        return false;
    }
    auto iter = group_members.begin();
    while( iter != group_members.end() )
    {
        string to_name = *iter;
        if( to_name == username ){
            iter++;
            continue;
        }
        if( username_to_fd.find(to_name) != username_to_fd.end() )
        {
            // 如果对方在线
            string s = to_msg.dump();
            int fd = username_to_fd[to_name];
            int ret = mysend(fd, s.c_str(), s.size(), 0);
            if( !ret ){
                printf("send massage from group %d to %s fail\n", groupid, to_name.data() );
            }
        }
        iter++;
    }

    // 写入群聊聊天记录
    ofstream out;
    out.open( path, ios::app );
    out << history.dump() << endl;
    out.close();
    return true;
}

// 开始传输文件，新建文件
bool send_file_begin( const json recv_msg, int &fileid, string &msg )
{
    string filename = recv_msg["filename"];
    string path;
    bool res = _chat_send_file_begin( filename, fileid, path, msg );
    if( !res )
    {
        printf("file %s add to database failed\n", filename.c_str() );
        return false;
    }
    FILE *fp = fopen( path.c_str(), "wb" );
    if( fp == NULL )
    {
        msg = "create file failed";
        printf("create file %s failed\n", filename.c_str() );
        return false;
    }
    fclose( fp );
}

// 聊天传输文件
bool chat_send_file( const json recv_msg, string &msg )
{
    int fileid = recv_msg["fileid"];
    bool eof = recv_msg["eof"];
    string path;
    bool res = _find_file_path( fileid, path, msg );
    if( !res )
    {
        printf("send file %d failed\n", fileid );
        return false;
    }
    if( !eof ){
        ofstream out( path, ios::app | ios::binary );
        if( out.is_open()!=true )
        {
            msg = "open file failed";
            printf("open file %d failed\n", fileid );
            return false;
        }
        out << ((string)recv_msg["content"]).c_str();
        out.close();
        return true;
    }
    else
    {
        // 文件接收完成
        string from_name = recv_msg["from"];
        string to_name = recv_msg["to"];
        json to_msg;
        json file;
        file["type"] = "file";
        file["file_id"] = fileid;
        string filename;
        bool res = _find_file_name( fileid, filename, msg );
        if( !res )
        {
            printf("find file %d name failed\n", fileid );
            return false;
        }
        file["file_name"] = filename;
        to_msg["debug"] = false;
        to_msg["command"] = "chat_recv_msg";
        to_msg["from"] = from_name;
        to_msg["to"] = to_name;
        to_msg["content"] = file;
        json history;
        history["name"] = from_name;
        history["type"] = "file";
        history["file_id"] = fileid;
        history["file_name"] = filename;
        history["read"] = false;
        string path;
        res = _find_chat_path( to_name, from_name, path, msg );
        if( !res )
        {
            printf("find chat path of %s and %s failed\n", from_name.c_str(), to_name.data() );
            return false;
        }
        if( username_to_fd.find(to_name) != username_to_fd.end() )
        {
            // 如果对方在线
            string s = to_msg.dump();
            int fd = username_to_fd[to_name];
            int ret = mysend(fd, s.c_str(), s.size(), 0);
            if( !ret ){
                printf("send massage from %s to %s fail\n", from_name.c_str(), to_name.data() );
                return false;
            }
            history["read"] = true;
            ofstream out;
            out.open( path, ios::app );
            out << history.dump() << endl;
            out.close();
            return true;
        }
        else
        {
            // 如果对方离线
            // 更新历史记录，记录未读
            ofstream out;
            out.open( path, ios::app );
            out << history.dump() << endl;
            out.close();

            // 数据库记录未读信息
            bool res = _update_unread( from_name, to_name, msg );
            return res;
        }
    }
}

// 群聊传输文件
bool group_send_file( const json recv_msg, string &msg )
{
    int fileid = recv_msg["fileid"];
    bool eof = recv_msg["eof"];
    string path;
    bool res = _find_file_path( fileid, path, msg );
    if( !res )
    {
        printf("send file %d failed\n", fileid );
        return false;
    }
    if( !eof ){
        ofstream out( path, ios::app | ios::binary );
        if( out.is_open()!=true )
        {
            msg = "open file failed";
            printf("open file %d failed\n", fileid );
            return false;
        }
        out << ((string)recv_msg["content"]).c_str();
        out.close();
        return true;
    }
    else
    {
        // 文件接收完成
        string username = recv_msg["from"];
        int groupid = recv_msg["group_id"];
        vector<string>group_members;
        _get_group_members( groupid, group_members, msg );
        json to_msg;
        json file;
        file["type"] = "file";
        file["file_id"] = fileid;
        string filename;
        bool res = _find_file_name( fileid, filename, msg );
        if( !res )
        {
            printf("find file %d name failed\n", fileid );
            return false;
        }
        file["file_name"] = filename;
        to_msg["debug"] = false;
        to_msg["command"] = "group_recv_msg";
        to_msg["from"] = username;
        to_msg["content"] = file;
        json history;
        history["name"] = username;
        history["type"] = "file";
        history["file_id"] = fileid;
        history["file_name"] = filename;
        string path;
        res = _find_group_path( groupid, path, msg );
        if( !res )
        {
            printf("find group %d path failed\n", groupid );
            return false;
        }
        auto iter = group_members.begin();
        while( iter != group_members.end() )
        {
            string to_name = *iter;
            if( to_name == username ){
                iter++;
                continue;
            }
            if( username_to_fd.find(to_name) != username_to_fd.end() )
            {
                // 如果对方在线
                string s = to_msg.dump();
                int fd = username_to_fd[to_name];
                int ret = mysend(fd, s.c_str(), s.size(), 0);
                if( !ret ){
                    printf("send massage from group %d to %s fail\n", groupid, to_name.data() );
                }
            }
            iter++;
        }

        // 写入群聊聊天记录
        ofstream out;
        out.open( path, ios::app );
        out << history.dump() << endl;
        out.close();
        return true;
    }
}

// 请求好友申请列表
bool list_friend_request( const json recv_msg, json &return_msg, string &msg )
{
    string username = recv_msg["username"];
    vector<string>list;
    bool res = _list_friend_request( username, list, msg );
    if( !res ){
        printf("get friend_request list fail\n");
        return false;
    }
    json j(list);
    return_msg["list"] = j;
    return true;
}

// 请求群聊邀请列表
bool list_group_invitation( const json recv_msg, json &return_msg, string &msg )
{
    string username = recv_msg["username"];
    vector<json>list;
    bool res = _list_group_invitation( username, list, msg );
    if( !res ){
        printf("get group_invitation list fail\n");
        return false;
    }
    json j(list);
    return_msg["list"] = j;
    return true;
}

// 请求群聊申请列表
bool list_group_request( const json recv_msg, json &return_msg, string &msg )
{
    string username = recv_msg["username"];
    vector<json>list;
    bool res = _list_group_request( username, list, msg );
    if( !res ){
        printf("get group_request list fail\n");
        return false;
    }
    json j(list);
    return_msg["list"] = j;
    return true;
}

// 获取聊天记录
bool get_chat_friend_history( const json recv_msg, json &return_msg, string &msg )
{
    string from_name = recv_msg["from"];
    string to_name = recv_msg["user"];
    string path;
    bool res = _find_chat_path( from_name, to_name, path, msg );
    if( !res )
    {
        printf("get chat_history path failed\n");
        return false;
    }
    vector<json>list;
    json j;
    string text;
    ifstream in;
    in.open( path, ios::in );
    if( in.is_open() != true )
    {
        msg = "open " + path + " failed";
        printf("open %s failed\n", path.c_str() );
        return false;
    }
    while( getline(in,text) ){
        j = json::parse( text );
        j.erase("read");
        list.push_back(j);
    }
    json jj(list);
    return_msg["list"] = jj;
    in.close();
    return true;
}

// 获取群聊记录
bool get_chat_group_history( const json recv_msg, json &return_msg, string &msg )
{
    int groupid = recv_msg["group_id"];
    string path;
    bool res = _find_group_path( groupid, path, msg );
    if( !res )
    {
        printf("get group_history path failed\n");
        return false;
    }
    vector<json>list;
    json j;
    string text;
    ifstream in;
    in.open( path, ios::in );
    if( in.is_open() != true )
    {
        msg = "open " + path + " failed";
        printf("open %s failed\n", path.c_str() );
        return false;
    }
    while( getline(in,text) ){
        j = json::parse( text );
        list.push_back(j);
    }
    json jj(list);
    return_msg["list"] = jj;
    in.close();
    return true;
}

// 获取好友列表
bool get_friends( const json recv_msg, json &return_msg, string &msg )
{
    string username = recv_msg["username"];
    vector<json>list;
    bool res = _get_friends( username, list, msg );
    if( !res ){
        printf("get friend list fail\n");
        return false;
    }
    json j(list);
//    json j = json::parse(list);
    return_msg["list"] = j;

    cout << return_msg << endl;
    return true;
}

// 获取群聊列表
bool get_groups( const json recv_msg, json &return_msg, string &msg )
{
    string username = recv_msg["username"];
    vector<json>list;
    bool res = _get_groups( username, list, msg );
    if( !res ){
        printf("get group list fail\n");
        return false;
    }
    json j(list);
    return_msg["list"] = j;
    return true;
}

json init_return_json( const json recv_msg )
{
    json j;
    j["debug"] = false; // remember to change with debug
    j["command"] = recv_msg["command"];
    j["status"] = false;
    j["msg"] = "";
    return j;
}

bool recv_file( const int fileid, const int fd )
{
    json to_msg;
    to_msg["debug"] = true;
    to_msg["command"] = "recv_file";
    to_msg["fileid"] = fileid;
    string filename, msg;
    bool res = _find_file_name( fileid, filename, msg );
    if( !res )
    {
        printf("find file %d name failed\n", fileid );
        return false;
    }

}

bool send_me_a_file( const json recv_msg, string &msg )
{

}
// bool recv_file( const int fileid, const int fd )
// {
//     // require a lock

//     char file_url[50] = {0};
//     string filename = find_filename( fileid, file_url ); // with mysql ... mzh
//     FILE *fp = fopen( file_url, "rb" );
//     if(fp == NULL){
//         return 0;
//     }
//     fp_fd new_file( filename, fileid, fp, fd );
//     alive_file.insert( new_file );

//     // require a lock
// }

void process_msg( const json recv_msg, const int fd )
{
    json return_msg = init_return_json( recv_msg );
    string msg;
    string cmd = recv_msg["command"];

    if( cmd == "login" ) // login
    {
        bool already_login = check_login( recv_msg, msg );
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
                GMutex * newmutex = new GMutex;
                g_mutex_init(newmutex);
                fd_to_mutex[fd] = newmutex;

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
        if( signup_res == false )
        {
            return_msg["msg"] = msg;
        }
        else
        {
            string username = recv_msg["username"];
            // add online_map
            username_to_fd[username] = fd;
            fd_to_username[fd] = username;
//            GMutex newmutex;
            GMutex * newmutex = new GMutex;
            g_mutex_init(newmutex);
            fd_to_mutex[fd] = newmutex;

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
        bool send_add_friend_request_res = send_add_friend_request( recv_msg, msg );
        if( send_add_friend_request_res )
        {
            return_msg["status"] = true;
        }
        else
        {
            return_msg["msg"] = "send request failed";
        }
    }
    else if( cmd == "send_invite_to_group_request" )
    {
        bool send_invite_to_group_res = send_invite_to_group_request( recv_msg, msg );
        if( send_invite_to_group_res )
        {
            return_msg["status"] = true;
        }
        else
        {
            return_msg["msg"] = msg;
        }
    }
    else if( cmd == "send_join_group_request" )
    {
        bool send_join_group_res = send_join_group_request( recv_msg, msg );
        if( send_join_group_res )
        {
            return_msg["status"] = true;
        }
        else
        {
            return_msg["msg"] = msg;
        }
    }
    else if( cmd == "alter_user_info" )
    {
        bool alter_user_info_res = alter_user_info( recv_msg, msg );
        if( alter_user_info_res )
        {
            return_msg["status"] = true;
        }
        else
        {
            return_msg["msg"] = msg;
        }
    }
    else if( cmd == "get_user_info" )
    {
        string val;
        bool get_user_info_res = get_user_info( recv_msg, val, msg );
        if( get_user_info_res )
        {
            return_msg["status"] = true;
            return_msg["val"] = val;
        }
        else
        {
            return_msg["msg"] = msg;
        }
    }
    else if( cmd == "list_user_like" )
    {
        vector<string>list;
        bool list_user_like_res = list_user_like( recv_msg, list, msg );
        if( list_user_like_res )
        {
            json j(list);
            return_msg["status"] = true;
            return_msg["list"] = j;
        }
        else
        {
            return_msg["msg"] = msg;
        }
    }
    else if( cmd == "list_group_like" )
    {
        vector<string>list;
        bool list_group_like_res = list_group_like( recv_msg, list, msg );
        if( list_group_like_res )
        {
            json j(list);
            return_msg["status"] = true;
            return_msg["list"] = j;
        }
        else
        {
            return_msg["msg"] = msg;
        }
    }
    else if( cmd == "send_add_friend_result" )
    {
        bool send_add_friend_result_res = send_add_friend_result( recv_msg, msg );
        if( send_add_friend_result_res )
        {
            return_msg["status"] = true;
        }
        else
        {
            return_msg["msg"] = msg;
        }
    }
    else if( cmd == "send_invite_to_group_result" )
    {
        bool send_invite_to_group_result_res = send_invite_to_group_result( recv_msg, msg );
        if( send_invite_to_group_result_res )
        {
            return_msg["status"] = true;
        }
        else
        {
            return_msg["msg"] = msg;
        }
    }
    else if( cmd == "send_join_group_result" )
    {
        bool send_join_group_result_res = send_join_group_result( recv_msg, msg );
        if( send_join_group_result_res )
        {
            return_msg["status"] = true;
        }
        else
        {
            return_msg["msg"] = msg;
        }
    }
    else if( cmd == "chat_send_msg" )
    {
        bool chat_send_msg_res = chat_send_msg( recv_msg, msg );
        if( chat_send_msg_res )
        {
            return_msg["status"] = true;
        }
        else
        {
            return_msg["msg"] = msg;
        }
    }
    else if( cmd == "group_send_msg" )
    {
        bool group_send_msg_res = group_send_msg( recv_msg, msg );
        if( group_send_msg_res )
        {
            return_msg["status"] = true;
        }
        else
        {
            return_msg["msg"] = msg;
        }
    }
    else if( cmd == "send_file_begin" )
    {
        int fileid;
        bool chat_send_file_begin_res = send_file_begin( recv_msg, fileid, msg );
        if( chat_send_file_begin_res )
        {
            return_msg["fileid"] = fileid;
            return_msg["status"] = true;
        }
        else
        {
            return_msg["msg"] = msg;
        }
    }
    else if( cmd == "chat_send_file" )
    {
        bool chat_send_file_res = chat_send_file( recv_msg, msg );
        if( chat_send_file_res )
        {
            return_msg["status"] = true;
        }
        else
        {
            return_msg["msg"] = msg;
        }
    }
    else if( cmd == "group_send_file" )
    {
        bool group_send_file_res = group_send_file( recv_msg, msg );
        if( group_send_file_res )
        {
            return_msg["status"] = true;
        }
        else
        {
            return_msg["msg"] = msg;
        }
    }
    else if( cmd == "send_me_a_file" ) // todo
    {
        bool send_me_a_file_res = send_me_a_file( recv_msg, msg );
        if( send_me_a_file_res )
        {
            return_msg["status"] = true;
        }
        else
        {
            return_msg["msg"] = msg;
        }
    }
    else if( cmd == "list_friend_request" )
    {
        bool list_friend_request_res = list_friend_request( recv_msg, return_msg, msg );
        if( list_friend_request_res )
        {
            return_msg["status"] = true;
        }
        else
        {
            return_msg["msg"] = msg;
        }
    }
    else if( cmd == "list_group_invitation" )
    {
        bool list_group_invitation_res = list_group_invitation( recv_msg, return_msg, msg );
        if( list_group_invitation_res )
        {
            return_msg["status"] = true;
        }
        else
        {
            return_msg["msg"] = msg;
        }
    }
    else if( cmd == "list_group_request" )
    {
        bool list_group_request_res = list_group_request( recv_msg, return_msg, msg );
        if( list_group_request_res )
        {
            return_msg["status"] = true;
        }
        else
        {
            return_msg["msg"] = msg;
        }
    }
    else if( cmd == "get_chat_friend_history" )
    {
        bool get_chat_friend_history_res = get_chat_friend_history( recv_msg, return_msg, msg );
        if( get_chat_friend_history_res )
        {
            return_msg["status"] = true;
        }
        else
        {
            return_msg["msg"] = msg;
        }
    }
    else if( cmd == "get_chat_group_history" )
    {
        bool get_chat_group_history_res = get_chat_group_history( recv_msg, return_msg, msg );
        if( get_chat_group_history_res )
        {
            return_msg["status"] = true;
        }
        else
        {
            return_msg["msg"] = msg;
        }
    }
    else if( cmd == "get_friends" )
    {
        bool get_friends_res = get_friends( recv_msg, return_msg, msg );
        if( get_friends_res )
        {
            return_msg["status"] = true;
        }
        else
        {
            return_msg["msg"] = msg;
        }
    }
    else if( cmd == "get_groups" )
    {
        bool get_groups_res = get_groups( recv_msg, return_msg, msg );
        if( get_groups_res )
        {
            return_msg["status"] = true;
        }
        else
        {
            return_msg["msg"] = msg;
        }
    }

    // send massage to client
    string s = return_msg.dump();
    // char *c = s.data();
    // int ret = send(fd, c, strlen(c), 0);
    int ret = mysend(fd, s.c_str(), s.size(), 0);
    if (ret == -1) {
        printf("send fail\n");
    } else {
        printf("send complete\n");
    }
}

const char debug_str[100] = "{\"debug\":true}";

// process file thread function
// void * file_thread_func( void *data )
// {
//     char buffer[1024];
//     int buffer_size;
//     json send_msg;
//     send_msg["command"] = "recv_file";
//     printf("file_thread_started\n");
//     while (1) {
//         // require lock;

//         auto iter = alive_file.begin();
//         while( iter != alive_file.end() )
//         {
//             int fd = (*iter).fd;
//             FILE *fp = (*iter).fp;
//             send_msg["filename"] = (*iter).filename;
//             send_msg["fileid"] = (*iter).fileid;
//             buffer_size = fread(buffer, 1, BUF_SIZE, fp);
//             if( buffer_size == 0 )
//             {
//                 send_msg["eof"] = true;
//                 string s = send_msg.dump();
//                 char *c = s.data;
//                 send( fd, c, strlen(c), 0 );
//                 fclose( fp );
//                 alive_file.erase( iter );
//                 continue;
//             }
//             else
//             {
//                 send_msg["eof"] = false;
//                 string s = buffer;
//                 send_msg["content"] = s;
//                 send( fd, buffer, buffer_size, 0);
//             }
//             iter ++;
//         }

//         // require lock

//     }
// }

// user functional thread function
void * thread_func(void * data)
{
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
                process_msg( j, fd );

//                int ret = send(fd, debug_str, strlen(debug_str), 0);
//                if (ret == -1) {
//                    printf("send fail\n");
//                } else {
//                    printf("send complete\n");
//                }
            }

            iter++;
        }

        g_mutex_unlock(&alive_socket_mutex);
    }
}

int main(int argc, char * argv[])
{

    my_connect();
    Setup_Database();


    new_floder();
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
    // g_thread_new("thread3", file_thread_func, nullptr);

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