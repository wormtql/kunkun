//
// Created by iatow on 19-8-29.
//

#include "INCLUDE.h"
#include "ClientUtils.h"

// 阻塞的 login
json ClientUtils::login_blocked(const std::string &username, const std::string &password) {
    // 阻塞接收

    Thread::stop_recv();

    json j;

    j["command"] = "login";
    j["username"] = username;
    j["password"] = password;

    Client::getIns()->send_string(j.dump());

    char buf[200];
    Client::getIns()->recv_blocked(buf);


    Thread::resume_recv();


    return json::parse(buf);
}

json ClientUtils::signup_blocked(const std::string &username, const std::string &password) {
    Thread::stop_recv();

    json j;
    j["command"] = "signup";
    j["username"] = username;
    j["password"] = password;

    Client::getIns()->send_string(j.dump());

    char buf[200];
    Client::getIns()->recv_blocked(buf);

    Thread::resume_recv();

    return json::parse(buf);
}