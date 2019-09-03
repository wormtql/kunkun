//
// Created by iatow on 19-9-3.
//


#include <iostream>
#include <string>
#include <cstring>
#include <gtk/gtk.h>
#include <mysql/mysql.h>
#include <vector>

#include "json.hpp"

#include "Database.h"

using std::string;
using std::vector;

using namespace std;

MYSQL *myconnect = NULL;
char database_username[64] = "root";
char database_userpwd[64] = "root";
int chartoint(char *s)
{
    int temp = 0;
    for(int i=0;s[i];i++)
    {
        temp = temp*10 + s[i]-'0';
    }
    return temp;
}
void rev(char *s)
{
    int len = strlen(s);
    for(int i=0,j=len-1;i<j;i++,j--)
    {
        std::swap(s[i], s[j]);
    }
}
char *inttochar(int d)
{
    char s[40] = "\0";
    int top = 0;
    while(d)
    {
        s[top++] = d%10+'0';
        d/=10;
    }

}
bool iscreate()
{
    char query_buf[4096] = "\0";
    MYSQL_RES *res;
    MYSQL_ROW row;
    int ans = 0;
    sprintf(query_buf,"SELECT * FROM information_schema.SCHEMATA where SCHEMA_NAME = 'kunkun';");
    if(mysql_query(myconnect,query_buf))
    {
        fprintf(stderr, "%s\n", mysql_error(myconnect));
        exit(1);
    }
    res = mysql_use_result(myconnect);
    while((row = mysql_fetch_row(res)) != NULL)
    {
        ans++;
    }
    if(ans == 0)
    {
        mysql_free_result(res);
        return 0;
    }
    mysql_free_result(res);
    return 1;
}
bool Setup_Database()
{
    if(iscreate()) {
        mysql_query(myconnect, "use kunkun;");
        return 1;
    }
    char query_buf[4096] = "\0";
    sprintf(query_buf, "CREATE SCHEMA kunkun default character set utf8 collate utf8_general_ci;");
    if(mysql_query(myconnect, query_buf))
    {
        fprintf(stderr, "%s\n", mysql_error(myconnect));
        exit(1);
    }
    mysql_query(myconnect, "use kunkun;");
    sprintf(query_buf, "CREATE TABLE alluser("
                       "username CHAR(20) PRIMARY KEY, "
                       "password CHAR(20) NOT NULL DEFAULT '', "
                       "gender CHAR(20) DEFAULT 'secret', "
                       "signature CHAR(40) NOT NULL DEFAULT '', "
                       "startime TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP"
                       ");");
    if(mysql_query(myconnect, query_buf))
    {
        fprintf(stderr, "%s\n", mysql_error(myconnect));
        exit(1);
    }
    sprintf(query_buf, "CREATE TABLE allgroup("
                       "groupid INT PRIMARY KEY AUTO_INCREMENT, "
                       "groupname CHAR(20) NOT NULL DEFAULT 'groupchat', "
                       "admin CHAR(20) NOT NULL DEFAULT '', "
                       "startime TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP, "
                       "chatrecord CHAR(40) NOT NULL DEFAULT ''"
                       ");");
    if(mysql_query(myconnect, query_buf))
    {
        fprintf(stderr, "%s\n", mysql_error(myconnect));
        exit(1);
    }
    sprintf(query_buf, "CREATE TABLE users("
                       "username1 CHAR(20), "
                       "username2 CHAR(20), "
                       "tag CHAR(20) DEFAULT 'my_friend', "
                       "unread CHAR(20) DEFAULT 'no', "
                       "startime TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP, "
                       "chatrecord CHAR(40), "
                       "PRIMARY KEY(username1, username2)"
                       ");");
    if(mysql_query(myconnect, query_buf))
    {
        fprintf(stderr, "%s\n", mysql_error(myconnect));
        exit(1);
    }
    sprintf(query_buf, "CREATE TABLE groups("
                       "username CHAR(20), "
                       "groupid CHAR(20), "
                       "tag CHAR(20) DEFAULT 'usual', "
                       "startime TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP, "
                       "PRIMARY KEY(username, groupid)"
                       ");");
    if(mysql_query(myconnect, query_buf))
    {
        fprintf(stderr, "%s\n", mysql_error(myconnect));
        exit(1);
    }
    sprintf(query_buf, "CREATE TABLE adduser("
                       "username_receive CHAR(20), "
                       "username_send CHAR(20), "
                       "startime TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP, "
                       "PRIMARY KEY(username_receive, username_send)"
                       ");");
    if(mysql_query(myconnect, query_buf))
    {
        fprintf(stderr, "%s\n", mysql_error(myconnect));
        exit(1);
    }
    sprintf(query_buf, "CREATE TABLE groupinvite("
                       "groupid CHAR(20), "
                       "username_send CHAR(20), "
                       "username_receive CHAR(20), "
                       "startime TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP, "
                       "PRIMARY KEY(groupid, username_send, username_receive)"
                       ");");
    if(mysql_query(myconnect, query_buf))
    {
        fprintf(stderr, "%s\n", mysql_error(myconnect));
        exit(1);
    }
    sprintf(query_buf, "CREATE TABLE groupapply("
                       "username CHAR(20), "
                       "groupid CHAR(20), "
                       "startime TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP, "
                       "PRIMARY KEY(username, groupid)"
                       ");");
    if(mysql_query(myconnect, query_buf))
    {
        fprintf(stderr, "%s\n", mysql_error(myconnect));
        exit(1);
    }
    sprintf(query_buf, "CREATE TABLE filelist("
                       "fileid INT PRIMARY KEY AUTO_INCREMENT, "
                       "filename CHAR(40), "
                       "fileaddress CHAR(20), "
                       "startime TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP"
                       ");");
    if(mysql_query(myconnect, query_buf))
    {
        fprintf(stderr, "%s\n", mysql_error(myconnect));
        exit(1);
    }
    return 1;
}
bool my_connect()
{
    myconnect = mysql_init(myconnect);
    if(mysql_real_connect(myconnect, "localhost", database_username, database_userpwd, NULL, 0, NULL, 0))
    {
        return TRUE;
    }
    else
    {
        myconnect = NULL;
        return FALSE;
    }
}
void my_disconnect()
{
    mysql_close(myconnect);
    myconnect = NULL;
}
bool is_user_exit(string name)
{
    if(myconnect == NULL)
    {
        my_connect();
    }
    char query_buf[4096] = "\0";
    MYSQL_RES *res;
    MYSQL_ROW row;
    sprintf(query_buf, "SELECT * FROM alluser WHERE username = '%s';", name.c_str());
    mysql_query(myconnect, query_buf);
    res = mysql_use_result(myconnect);
    row = mysql_fetch_row(res);
    if(row == NULL)
    {
        mysql_free_result(res);
        return 0;
    }
    mysql_free_result(res);
    return 1;
}
bool is_group_exit(int id)
{
    if(myconnect == NULL)
    {
        my_connect();
    }
    char query_buf[4096] = "\0";
    MYSQL_RES *res;
    MYSQL_ROW row;
    sprintf(query_buf, "SELECT * FROM allgroup WHERE groupid = %d;", id);
    mysql_query(myconnect, query_buf);
    res = mysql_use_result(myconnect);
    row = mysql_fetch_row(res);
    if(row == NULL)
    {
        mysql_free_result(res);
        return 0;
    }
    mysql_free_result(res);
    return 1;
}
bool _login(string user_name, string user_password, string &msg)
{
    if(myconnect == NULL)
    {
        my_connect();
    }
    char query_buf[4096] = "\0";
    MYSQL_RES *res;
    MYSQL_ROW row;
    sprintf(query_buf, "SELECT * FROM alluser where username = '%s';", user_name.c_str());
    printf("query: %s\n", query_buf);
    mysql_query(myconnect, query_buf);
    res = mysql_use_result(myconnect);
    fprintf(stderr, "%s\n", mysql_error(myconnect));
    row = mysql_fetch_row(res);
    if(row == NULL)
    {
        msg = "username not found!";
        mysql_free_result(res);
        return 0;
    }
//    row = mysql_fetch_row(res);
    if(strcmp(user_password.c_str(), row[1]) != 0)
    {
        msg = "username or password error!";
        mysql_free_result(res);
        return 0;
    }
    msg = "successful login!";
    mysql_free_result(res);
    return 1;
}
bool _signup(string user_name, string user_password, string &msg)
{
    if(myconnect == NULL)
    {
        my_connect();
    }
    char query_buf[4096] = "\0";
    MYSQL_RES *res;
    MYSQL_ROW row;
    sprintf(query_buf, "SELECT * FROM alluser where username = '%s';", user_name.c_str());
    mysql_query(myconnect, query_buf);
    res = mysql_use_result(myconnect);
//    std::cerr << mysql_err
    fprintf(stderr, "%s\n", mysql_error(myconnect));
    row = mysql_fetch_row(res);
    if(row != NULL)
    {
        msg = "username already has been used!";
        mysql_free_result(res);
        return 0;
    }
    sprintf(query_buf, "INSERT INTO alluser "
                       "(username, password) "
                       "VALUES "
                       "('%s', '%s');", user_name.c_str(), user_password.c_str());
    mysql_query(myconnect, query_buf);
    msg = "successful signup!";
    mysql_free_result(res);
    return 1;
}
bool _create_group(string groupname, string creater, int &groupid, string &msg)
{
    if(myconnect == NULL)
    {
        my_connect();
    }
    char query_buf[4096] = "\0";
    MYSQL_RES *res;
    MYSQL_ROW row;
    sprintf(query_buf, "INSERT INTO allgroup "
                       "(groupname, admin) "
                       "VALUES "
                       "('%s', '%s');", groupname.c_str(), creater.c_str());
    mysql_query(myconnect, query_buf);
    sprintf(query_buf, "SELECT groupid FROM allgroup ORDER BY groupid DESC;");
    mysql_query(myconnect, query_buf);
    res = mysql_use_result(myconnect);
    row = mysql_fetch_row(res);
    int id = chartoint(row[0]);
    mysql_free_result(res);
    groupid = id;
    char address[40] = "\0";
    sprintf(address, "group/%s", row[0]);
    sprintf(query_buf, "UPDATE allgroup SET chatrecord = '%s' WHERE groupid = %d;", address, id);
    mysql_query(myconnect, query_buf);
    return 1;
}
bool _rename_group(int id, string username, string new_groupname, string &msg)
{
    if(myconnect == NULL)
    {
        my_connect();
    }
    char query_buf[4096] = "\0";
    MYSQL_RES *res;
    MYSQL_ROW row;
    sprintf(query_buf, "SELECT * FROM allgroup WHERE groupid = %d AND admin = '%s';", id, username.c_str());
    mysql_query(myconnect, query_buf);
    res = mysql_use_result(myconnect);
    row = mysql_fetch_row(res);
    if(row == NULL)
    {
        msg = "unauthorized!";
        mysql_free_result(res);
        return 0;
    }
    mysql_free_result(res);
    sprintf(query_buf, "UPDATE allgroup SET groupname = '%s' WHERE groupid = %d;", new_groupname.c_str(), id);
    mysql_query(myconnect, query_buf);
    return 1;
}
bool _send_add_friend_request(string from_name, string to_name, string &msg)
{
    if(myconnect == NULL)
    {
        my_connect();
    }
    char query_buf[4096] = "\0";
    MYSQL_RES *res;
    MYSQL_ROW row;
    if(!is_user_exit(to_name))
    {
        msg = "user not found!";
        return 0;
    }
    sprintf(query_buf, "SELECT * FROM users WHERE username1 = '%s' AND username2 = '%s';", from_name.c_str(), to_name.c_str());
    mysql_query(myconnect, query_buf);
    res = mysql_use_result(myconnect);
    row = mysql_fetch_row(res);
    if(row != NULL)
    {
        msg = "already in your friendlist!";
        mysql_free_result(res);
        return 0;
    }
    mysql_free_result(res);
    sprintf(query_buf, "INSERT INTO adduser "
                       "(username_receive, username_send) "
                       "VALUES "
                       "('%s', '%s');", to_name.c_str(), from_name.c_str());
    mysql_query(myconnect, query_buf);
    return 1;
}
bool _send_invite_to_group_request(string invite_username, string to_username, int groupid, string &msg)
{
    if(myconnect == NULL)
    {
        my_connect();
    }
    char query_buf[4096] = "\0";
    MYSQL_RES *res;
    MYSQL_ROW row;
    if(!is_user_exit(to_username))
    {
        msg = "user not found!";
        return 0;
    }
    if(!is_group_exit(groupid))
    {
        msg = "group not found!";
        return 0;
    }
    sprintf(query_buf, "SELECT * FROM groups WHERE username = '%s' AND groupid = %d;", to_username.c_str(), groupid);
    mysql_query(myconnect, query_buf);
    res = mysql_use_result(myconnect);
    row = mysql_fetch_row(res);
    if(row != NULL)
    {
        msg = "already in this group!";
        mysql_free_result(res);
        return 0;
    }
    mysql_free_result(res);
    sprintf(query_buf, "INSERT INTO groupinvite "
                       "(groupid, username_send, username_receive) "
                       "VALUES "
                       "(%d, '%s', '%s');", groupid, invite_username.c_str(), to_username.c_str());
    mysql_query(myconnect, query_buf);
    return 1;
}
bool _send_join_group_request(string username, int groupid, string &msg)
{
    if(myconnect == NULL)
    {
        my_connect();
    }
    char query_buf[4096] = "\0";
    MYSQL_RES *res;
    MYSQL_ROW row;
    if(!is_group_exit(groupid))
    {
        msg = "group not found!";
        return 0;
    }
    sprintf(query_buf, "SELECT * FROM groups WHERE username = '%s' AND groupid = %d;", username.c_str(), groupid);
    mysql_query(myconnect, query_buf);
    res = mysql_use_result(myconnect);
    row = mysql_fetch_row(res);
    if(row != NULL)
    {
        msg = "already in this group!";
        mysql_free_result(res);
        return 0;
    }
    mysql_free_result(res);
    sprintf(query_buf, "INSERT INTO groupapply "
                       "(username, groupid) "
                       "VALUES "
                       "('%s', %d);", username.c_str(), groupid);
    mysql_query(myconnect, query_buf);
    return 1;
}
bool _alter_user_info(string username, string filed, string new_val, string &msg)
{
    if(myconnect == NULL)
    {
        my_connect();
    }
    char query_buf[4096] = "\0";
    sprintf(query_buf, "UPDATE alluser SET %s = '%s' WHERE username = '%s';",filed.c_str(), new_val.c_str(), username.c_str());
    mysql_query(myconnect, query_buf);
    return 1;
}
bool _get_user_info(string username, string field, string &val, string &msg)
{
    if(myconnect == NULL)
    {
        my_connect();
    }
    char query_buf[4096] = "\0";
    MYSQL_RES *res;
    MYSQL_ROW row;
    sprintf(query_buf, "SELECT %s FROM alluser WHERE username = '%s';", field.c_str(), username.c_str());
    mysql_query(myconnect, query_buf);
    res = mysql_use_result(myconnect);
    row = mysql_fetch_row(res);
    val = string(row[0]);
    mysql_free_result(res);
    return 1;
}
bool _list_user_like(string query_str, vector<string> &user_list, string &msg)
{
    if(myconnect == NULL)
    {
        my_connect();
    }
    char query_buf[4096] = "\0";
    MYSQL_RES *res;
    MYSQL_ROW row;
    user_list.clear();
    sprintf(query_buf, "SELECT username FROM alluser WHERE username like '%%%s%%';", query_str.c_str());
    mysql_query(myconnect, query_buf);
    res = mysql_use_result(myconnect);
    while((row = mysql_fetch_row(res)) != NULL)
    {
        user_list.push_back(string(row[0]));
    }
    mysql_free_result(res);
    return 1;
}
bool _list_group_like(string query_str, vector<string>&group_list, string &msg)
{
    if(myconnect == NULL)
    {
        my_connect();
    }
    char query_buf[4096] = "\0";
    MYSQL_RES *res;
    MYSQL_ROW row;
    group_list.clear();
    sprintf(query_buf, "SELECT groupname FROM allgroup WHERE groupname like '%%%s%%';", query_str.c_str());
    mysql_query(myconnect, query_buf);
    res = mysql_use_result(myconnect);
    while((row = mysql_fetch_row(res)) != NULL)
    {
        group_list.push_back(string(row[0]));
    }
    mysql_free_result(res);
    return 1;
}
bool _send_add_friend_result(string username1, string username2, string &msg)
{
    if(myconnect == NULL)
    {
        my_connect();
    }
    char query_buf[4096] = "\0";
    char address[40] = "\0";
    MYSQL_RES *res;
    MYSQL_ROW row;
    sprintf(address, "chat/%s_%s", username1.c_str(), username2.c_str());
    sprintf(query_buf, "INSERT INTO users "
                       "(username1, username2, chatrecord) "
                       "VALUES "
                       "('%s', '%s', '%s');", username1.c_str(), username2.c_str(), address);
    mysql_query(myconnect, query_buf);
    sprintf(query_buf, "INSERT INTO users "
                       "(username1, username2, chatrecord) "
                       "VALUES "
                       "('%s', '%s', '%s');", username2.c_str(), username1.c_str(), address);
    mysql_query(myconnect, query_buf);
    sprintf(query_buf, "DELETE FROM adduser "
                       "WHERE username1 = '%s' AND "
                       "username2 = '%s';", username1.c_str(), username2.c_str());
    mysql_query(myconnect, query_buf);
    sprintf(query_buf, "DELETE FROM adduser "
                       "WHERE username1 = '%s' AND "
                       "username2 = '%s';", username2.c_str(), username1.c_str());
    mysql_query(myconnect, query_buf);
    return 1;
}

bool _send_add_group_result(int groupid,string username, string &msg)
{
    if(myconnect = NULL)
    {
        my_connect();
    }
    char query_buf[4096] = "\0";
    sprintf(query_buf, "INSERT INTO groups "
                       "(username, groupid) "
                       "VALUES "
                       "('%s', %d);", username.c_str(), groupid);
    mysql_query(myconnect, query_buf);
    sprintf(query_buf, "DELETE FROM groupinvite WHERE groupid = %d AND username_receive = '%s';", groupid, username.c_str());
    mysql_query(myconnect, query_buf);
    sprintf(query_buf, "DELETE FROM groupapply WHERE groupid = %d AND username = '%s';", groupid, username.c_str());
    mysql_query(myconnect, query_buf);
    return 1;
}
bool _chat_send_file_begin(string filename, int &fileid, string &path, string &msg)
{
    if(myconnect == NULL)
    {
        my_connect();
    }
    char query_buf[4096] = "\0";
    char address[40] = "\0";
    MYSQL_RES *res;
    MYSQL_ROW row;
    sprintf(query_buf, "INSERT INTO filelist "
                       "(filename) "
                       "VALUES "
                       "('%s');", filename.c_str());
    mysql_query(myconnect, query_buf);
    sprintf(query_buf, "SELECT fileid FROM filelist ORDER BY fileid DESC;");
    mysql_query(myconnect, query_buf);
    res = mysql_use_result(myconnect);
    row = mysql_fetch_row(res);
    fileid = chartoint(row[0]);
    mysql_free_result(res);
    sprintf(address, "files/%d_%s", fileid, filename.c_str());
    path = string(address);
    sprintf(query_buf, "UPDATE filelist SET fileaddress = '%s' WHERE fileid = %d;", address, fileid);
    mysql_query(myconnect, query_buf);
    return 1;
}
bool _list_friend_request(string username, vector<string>&list, string &msg)
{
    if(myconnect == NULL)
    {
        my_connect();
    }
    list.clear();
    char query_buf[4096] = "\0";
    MYSQL_RES *res;
    MYSQL_ROW row;
    sprintf(query_buf, "SELECT username_send FROM adduser WHERE username_receive = '%s';", username.c_str());
    mysql_query(myconnect, query_buf);
    res = mysql_use_result(myconnect);
    while((row = mysql_fetch_row(res)) != NULL)
    {
        list.push_back(string(row[0]));
    }
    mysql_free_result(res);
    return 1;
}
//struct list_group_invitation_struct
//{
//    string name,boss;
//    int id;
//    list_group_invitation_struct(){}
//    list_group_invitation_struct(string name, string boss, int id) : name(name), boss(boss), id(id) {}
//};
bool _list_group_invitation(string username, vector<json>&list, string &msg)
{
    if(myconnect == NULL)
    {
        my_connect();
    }
    list.clear();
    char query_buf[4096] = "\0";
    MYSQL_RES *res;
    MYSQL_ROW row;
    sprintf(query_buf, "SELECT groupname, username_send, allgroup.groupid FROM allgroup, groupinvite WHERE allgroup.groupid = groupinvite.groupid AND username_receive = '%s';", username.c_str());
    mysql_query(myconnect, query_buf);
    res = mysql_use_result(myconnect);
    while((row = mysql_fetch_row(res)) != NULL)
    {
        json j;
        j["name"] = row[0];
        j["boss"] = row[1];
        j["id"] = chartoint(row[2]);
        list.push_back(j);
//        list.push_back(list_group_invitation_struct(string(row[0]), string(row[1]), chartoint(row[2])));
    }
    mysql_free_result(res);
    return 1;
}
//struct list_group_request_struct
//{
//    string username;
//    int group_id;
//    list_group_request_struct(){}
//    list_group_request_struct(string username, int group_id) : username(username), group_id(group_id) {}
//};
//bool _list_group_request(string username, vector<list_group_request_struct>&list, string &msg)
//{
//    if(myconnect)
//    {
//        my_connect();
//    }
//    list.clear();
//    char query_buf[4096] = "\0";
//    MYSQL_RES *res;
//    MYSQL_ROW row;
//    sprintf(query_buf, "SELECT username, groupapply.groupid FROM allgroup, groupapply WHERE allgroup.groupid = groupapply.groupid AND admin = '%s';", username.c_str());
//    mysql_query(myconnect, query_buf);
//    res = mysql_use_result(myconnect);
//    while((row = mysql_fetch_row(res)) != NULL)
//    {
//        list.push_back(list_group_request_struct(string(row[0]), chartoint(row[1])));
//    }
//    mysql_free_result(res);
//    return 1;
//}
bool _list_group_request(string username, vector<json>&list, string &msg)
{
    if(myconnect)
    {
        my_connect();
    }
    list.clear();
    char query_buf[4096] = "\0";
    MYSQL_RES *res;
    MYSQL_ROW row;
    sprintf(query_buf, "SELECT username, groupapply.groupid FROM allgroup, groupapply WHERE allgroup.groupid = groupapply.groupid AND admin = '%s';", username.c_str());
    mysql_query(myconnect, query_buf);
    res = mysql_use_result(myconnect);
    while((row = mysql_fetch_row(res)) != NULL)
    {
        json j;
        j["username"] = string(row[0]);
        j["groupid"] = chartoint(row[1]);
        list.push_back(j);
    }
    mysql_free_result(res);
    return 1;
}
//struct get_friends_struct
//{
//    string name, tag;
//    get_friends_struct(){}
//    get_friends_struct(string name, string tag) : name(name), tag(tag){}
//};
//bool _get_friends(string username, vector<get_friends_struct>&list, string &msg)
//{
//    if(myconnect == NULL)
//    {
//        my_connect();
//    }
//    char query_buf[4096] = "\0";
//    MYSQL_RES *res;
//    MYSQL_ROW row;
//    sprintf(query_buf, "SELECT username2, tag FROM users WHERE username1 = '%s';", username.c_str());
//    mysql_query(myconnect, query_buf);
//    res = mysql_use_result(myconnect);
//    while((row = mysql_fetch_row(res)) != NULL)
//    {
//        list.push_back(get_friends_struct(string(row[0]), string(row[1])));
//    }
//    mysql_free_result(res);
//    return 1;
//}
bool _get_friends(string username, vector<json>&list, string &msg)
{
    if(myconnect == NULL)
    {
        my_connect();
    }
    char query_buf[4096] = "\0";
    MYSQL_RES *res;
    MYSQL_ROW row;
    sprintf(query_buf, "SELECT username2, tag FROM users WHERE username1 = '%s';", username.c_str());
    mysql_query(myconnect, query_buf);
    res = mysql_use_result(myconnect);
    while((row = mysql_fetch_row(res)) != NULL)
    {
        json j;
        j["name"] = string(row[0]);
        j["tag"] = string(row[1]);
        list.push_back(j);

        cout << j << endl;
    }
    mysql_free_result(res);
    return 1;
}
//struct get_groups_struct
//{
//    string name, tag;
//    get_groups_struct(){}
//    get_groups_struct(string name, string tag) : name(name), tag(tag){}
//};
//bool get_groups(string username, vector<get_groups_struct>&list, string &msg)
//{
//
//}
bool _get_groups(string username, vector<json>&list, string &msg)
{
    if(myconnect == NULL)
    {
        my_connect();
    }
    list.clear();
    char query_buf[4096] = "\0";
    MYSQL_RES *res;
    MYSQL_ROW row;
    sprintf(query_buf, "SELECT groupname, tag FROM allgroup, groups WHERE allgroup.groupid = goups.groupid;");
    mysql_query(myconnect,query_buf);
    res = mysql_use_result(myconnect);
    while((row = mysql_fetch_row(res)) != NULL)
    {
        json j;
        j["name"] = string(row[0]);
        j["tag"] = string(row[1]);
        list.push_back(j);
    }
    mysql_free_result(res);
    return 1;
}
bool _find_groupname(int groupid, string &groupname, string &msg)
{
    if(myconnect == NULL)
    {
        my_connect();
    }
    char query_buf[4096] = "\0";
    MYSQL_RES *res;
    MYSQL_ROW row;
    sprintf(query_buf, "SELECT groupname FROM allgroup WHERE groupid = %d;", groupid);
    mysql_query(myconnect, query_buf);
    res = mysql_use_result(myconnect);
    row = mysql_fetch_row(res);
    if(row == NULL)
    {
        mysql_free_result(res);
        msg = "group not found!";
        return 0;
    }
    groupname = string(row[0]);
    mysql_free_result(res);
    return 1;
}
bool _find_group_creater(int groupid, string &creater, string &msg)
{
    if(myconnect == NULL)
    {
        my_connect();
    }
    char query_buf[4096] = "\0";
    MYSQL_RES *res;
    MYSQL_ROW row;
    sprintf(query_buf, "SELECT admin FROM allgroup WHERE groupid = %d;", groupid);
    mysql_query(myconnect, query_buf);
    res = mysql_use_result(myconnect);
    row = mysql_fetch_row(res);
    if(row == NULL)
    {
        mysql_free_result(res);
        msg = "group not found!";
        return 0;
    }
    creater = string(row[0]);
    mysql_free_result(res);
    return 1;
}
bool _find_chat_path(string username1, string username2, string &path, string &msg)
{
    if(myconnect == NULL)
    {
        my_connect();
    }
    char query_buf[4096] = "\0";
    MYSQL_RES *res;
    MYSQL_ROW row;
    sprintf(query_buf, "SELECT chatrecord FROM users WHERE username1 = '%s' AND username2 = '%s';", username1.c_str(), username2.c_str());
    mysql_query(myconnect, query_buf);
    res = mysql_use_result(myconnect);
    row = mysql_fetch_row(res);
    if(row == NULL)
    {
        mysql_free_result(res);
        msg = "record not found!";
        return 0;
    }
    path = string(row[0]);
    mysql_free_result(res);
    return 1;
}
bool _find_group_path(int groupid, string &path, string &msg)
{
    if(myconnect == NULL)
    {
        my_connect();
    }
    char query_buf[4096] = "\0";
    MYSQL_RES *res;
    MYSQL_ROW row;
    sprintf(query_buf, "SELECT chatrecord FROM allgroup WHERE groupid = %d;", groupid);
    mysql_query(myconnect, query_buf);
    res = mysql_use_result(myconnect);
    row = mysql_fetch_row(res);
    if(row == NULL)
    {
        mysql_free_result(res);
        msg = "record not found!";
        return 0;
    }
    path = string(row[0]);
    mysql_free_result(res);
    return 1;
}
bool _update_unread( string from_username, string to_username, string &msg )
{
    if(myconnect == NULL)
    {
        my_connect();
    }
    char query_buf[4096] = "\0";
    MYSQL_RES *res;
    MYSQL_ROW row;
    sprintf(query_buf, "UPDATE users SET unread = 'yes' WHERE username1 = '%s' AND username2 = '%s';", to_username.c_str(), from_username.c_str());
    mysql_query(myconnect, query_buf);
    return 1;
}
bool _get_group_members( int groupid, vector<string>&members, string &msg )
{
    if(myconnect == NULL)
    {
        my_connect();
    }
    members.clear();
    char query_buf[4096] = "\0";
    MYSQL_RES *res;
    MYSQL_ROW row;
    sprintf(query_buf, "SELECT username FROM groups WHERE groupid = %d;", groupid);
    mysql_query(myconnect, query_buf);
    res = mysql_use_result(myconnect);
    while((row = mysql_fetch_row(res)) != NULL)
    {
        members.push_back(string(row[0]));
    }
    mysql_free_result(res);
    return 1;
}
bool _find_file_path( int fileid, string &path, string &msg )
{
    if(myconnect == NULL)
    {
        my_connect();
    }
    char query_buf[4096] = "\0";
    MYSQL_RES *res;
    MYSQL_ROW row;
    sprintf(query_buf, "SELECT fileadress FROM filelist WHERE fileid = %d;", fileid);
    mysql_query(myconnect, query_buf);
    res = mysql_use_result(myconnect);
    row = mysql_fetch_row(res);
    if(row == NULL)
    {
        mysql_free_result(res);
        msg = "file not found!";
        return 0;
    }
    path = string(row[0]);
    mysql_free_result(res);
    return 1;
}
bool _find_file_name( int fileid, string &filename, string &msg )
{
    if(myconnect == NULL)
    {
        my_connect();
    }
    char query_buf[4096] = "\0";
    MYSQL_RES *res;
    MYSQL_ROW row;
    sprintf(query_buf, "SELECT filename FROM filelist WHERE fileid = %d;", fileid);
    mysql_query(myconnect, query_buf);
    res = mysql_use_result(myconnect);
    row = mysql_fetch_row(res);
    if(row == NULL)
    {
        mysql_free_result(res);
        msg = "file not found!";
        return 0;
    }
    filename = string(row[0]);
    mysql_free_result(res);
    return 1;
}
//int main() {
//    my_connect();
//    Setup_Database();
//    return 0;
//}