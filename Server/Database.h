//
// Created by iatow on 19-9-3.
//

#ifndef SERVER_DATABASE_H
#define SERVER_DATABASE_H

using std::string;
using std::vector;
using json = nlohmann::json;

int chartoint(char *s);

void rev(char *s);

char *inttochar(int d);

bool iscreate();

bool Setup_Database();

bool my_connect();

void my_disconnect();

bool is_user_exit(std::string name);

bool is_group_exit(int id);

bool _login(string user_name, string user_password, string &msg);

bool _signup(string user_name, string user_password, string &msg);

bool _create_group(string groupname, string creater, int &groupid, string &msg);

bool _rename_group(int id, string username, string new_groupname, string &msg);

bool _send_add_friend_request(string from_name, string to_name, string &msg);

bool _send_invite_to_group_request(string invite_username, string to_username, int groupid, string &msg);

bool _send_join_group_request(string username, int groupid, string &msg);

bool _alter_user_info(string username, string filed, string new_val, string &msg);

bool _get_user_info(string username, string field, string &val, string &msg);

bool _list_user_like(string query_str, vector<string> &user_list, string &msg);

bool _list_group_like(string query_str, vector<string>&group_list, string &msg);

bool _send_add_friend_result(string username1, string username2, string &msg);

bool _send_add_group_result(int groupid,string username, string &msg);

bool _chat_send_file_begin(string filename, int &fileid, string &path, string &msg);

bool _list_friend_request(string username, vector<string>&list, string &msg);

struct list_group_invitation_struct;

bool _list_group_invitation(string username, vector<json>&list, string &msg);

bool _list_group_request(string username, vector<json>&list, string &msg);

bool _get_friends(string username, vector<json>&list, string &msg);

bool _get_groups(string username, vector<json>&list, string &msg);

bool _find_groupname(int groupid, string &groupname, string &msg);

bool _find_group_creater(int groupid, string &creater, string &msg);

bool _find_chat_path(string username1, string username2, string &path, string &msg);

bool _find_group_path(int groupid, string &path, string &msg);

bool _update_unread( string from_username, string to_username, string &msg );

bool _get_group_members( int groupid, vector<string>&members, string &msg );

bool _find_file_path( int fileid, string &path, string &msg );

bool _find_file_name( int fileid, string &filename, string &msg );



#endif //SERVER_DATABASE_H
