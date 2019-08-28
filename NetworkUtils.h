//
// Created by iatow on 19-8-28.
//

#ifndef HOMEWORK_NETWORKUTILS_H
#define HOMEWORK_NETWORKUTILS_H


class NetworkUtils {
public:
    static bool login(const std::string & username, const std::string & pw, std::string & ret);

    static bool signup(const std::string & username, const std::string & pw, const std::string & cfm_pw, std::string & ret);
};


#endif //HOMEWORK_NETWORKUTILS_H
