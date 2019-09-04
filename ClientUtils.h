//
// Created by iatow on 19-8-29.
//

#ifndef KUNKUN_CLIENTUTILS_H
#define KUNKUN_CLIENTUTILS_H


class ClientUtils {
private:
    static char buf[20000];
public:

    /*
     * login
     *
     */
    static json login_blocked(const std::string & username, const std::string & password);


    /*
     * signup
     *
     */
    static json signup_blocked(const std::string & username, const std::string & password);


    /*
     * create group
     *
     */
    static json create_group(const std::string & who, const std::string & group_name);


    /*
     * rename group
     *
     */
    static json rename_group_blocked(int group_id, const std::string & new_name, const std::string & who);


    /*
     *
     *
     */
    static json send_add_friend_request(const std::string & from, const std::string & to);


    /*
     * set user info
     *
     */
    static json alter_user_info(const std::string & who, const std::string & field, const std::string & new_val);


    /*
     * get user info
     *
     */
    static std::string get_user_info(const std::string & who, const std::string & field);


    /*
     * 获得用户名包含query_str的用户列表
     *
     */
    static json get_list_user_like_blocked(const std::string & query_str);


    /*
     * 获取群名称或者群号包含query_str的群列表
     *
     */
    static json get_list_group_like_blocked(const std::string &query_str);


    /*
     * 获取who的好友列表
     *
     */
    static json get_friends(const std::string & who);


    /*
     * 获取who的群组列表
     *
     */
    static json get_groups(const std::string & who);


    /*
     * 给好友发送信息
     *
     */
//    static void send_text_to_friend(const std::string & from, const std::string & to, const std::string & content);


    /*
     * 给群聊发送信息
     *
     */
//    static void send_text_to_group(const std::string & from, const std::string & group_id, const std::string & content);


    /*
     * 获得好友的历史记录
     *
     */
    static json get_chat_friend_history(const std::string & from, const std::string & user);


    /*
     * 获得群聊的历史纪录
     *
     */
    static json get_chat_group_history(const std::string & from, const std::string & group_id);


//    static void request_add_friend(const std::string & from, const std::string & to);


    static void request_join_group(const std::string & from, const std::string & group_id);


    static void send_file_begin(const std::string & filename, SocketCallback callback);


    static void chat_send_file(const std::string & file_id, str from, str to, bool eof, char * content, int size);


    static void group_send_file(str fileid, str from, str group_id, bool eof, char * content, int size);
//    static void group_send_file_begin(str from, str group_id, str filename, SocketCallback callback);


    static void send_me_a_file(str fileid);


    static json list_friend_request(str username);


    static json list_group_invitation(str username);


    static json list_group_request(str username);


    static json send_add_friend_result(str from, str to, bool accept);


    static json send_invite_to_group_result(str whom, str group_id, bool accept);


    static json send_join_group_result(str who, str group_id, bool accept);


//    static json send_add_friend_result(const std::string & from, const std::string & to, bool accept);


    static json chat_send_msg(const std::string & from, const std::string & to, const std::string & msg);


    static json group_send_msg(const std::string & from, const std::string & group_id, const std::string & msg);


    static json ls_unread_msg(str who);

//    static json chat_send_file_begin(const std::string & sender, const std::string & recipient, const std::string & filename, SocketCallback callback);


//    static json chat_send_file(const std::string & file_id, const std::string & file_content, bool eof, SocketCallback callback);

    // todo ...
};


#endif //KUNKUN_CLIENTUTILS_H
