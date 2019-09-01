//
// Created by iatow on 19-8-29.
//

#ifndef KUNKUN_CLIENTUTILS_H
#define KUNKUN_CLIENTUTILS_H


class ClientUtils {
private:

public:

    static json login_blocked(const std::string & username, const std::string & password);


    static json signup_blocked(const std::string & username, const std::string & password);


    static json create_group_blocked(const std::string & who, const std::string & group_name);


    static json rename_group_blocked(int group_id, const std::string & new_name, const std::string & who);

    
    static json send_add_friend_request_blocked(const std::string & sender, const std::string & recipient);


    static json send_add_friend_result_blocked(const std::string & applier, const std::string & recipient, bool result);


    static json chat_send_msg_blocked(const std::string & sender, const std::string & recipient, const std::string & msg);


    static json group_send_msg_blocked(const std::string & sender, const std::string & group_id, const std::string & msg);


    static json chat_send_file_begin_blocked(const std::string & sender, const std::string & recipient, const std::string & filename);


    static json chat_send_file_blocked(const std::string & file_id, const std::string & file_content, bool eof);
    // todo ...
};


#endif //KUNKUN_CLIENTUTILS_H
