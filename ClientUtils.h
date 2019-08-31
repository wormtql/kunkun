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
    static json create_group_blocked(const std::string & who, const std::string & group_name);


    /*
     * rename group
     *
     */
    static json rename_group_blocked(int group_id, const std::string & new_name, const std::string who);


    /*
     * set user info
     *
     */
    static json alter_user_info(const std::string & who, const std::string & field, const std::string new_val);


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
    static void send_text_to_friend(const std::string & from, const std::string & to, const std::string & content);


    /*
     * 给群聊发送信息
     *
     */
    static void send_text_to_group(const std::string & from, const std::string & group_id, const std::string & content);


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


    static void request_add_friend(const std::string & from, const std::string & to);


    static void request_join_group(const std::string & from, const std::string & group_id);

    // todo ...
};


#endif //KUNKUN_CLIENTUTILS_H
