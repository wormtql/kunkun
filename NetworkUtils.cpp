//
// Created by iatow on 19-8-28.
//

#include "INCLUDE.h"

#include "NetworkUtils.h"


bool NetworkUtils::login(const std::string &username, const std::string &pw, std::string &ret) {
    return true;
//    return false;
}

bool NetworkUtils::signup(const std::string &username, const std::string &pw, const std::string &cfm_pw,
                          std::string &ret) {
    ret = "username existed";
    return false;
}