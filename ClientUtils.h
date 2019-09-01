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
    // todo ...
};


#endif //KUNKUN_CLIENTUTILS_H
