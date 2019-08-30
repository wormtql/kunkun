//
// Created by iatow on 19-8-29.
//

#include "INCLUDE.h"
#include "ClientUtils.h"

void ClientUtils::login(const std::string &username, const std::string &password, SocketCallback callback) {
    Thread::set_call_back(callback);

    json j;

    j["command"] = "login";
    j["username"] = username;
    j["password"] = password;

    Client::getIns()->send_string(j.dump());
}
