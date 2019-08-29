//
// Created by iatow on 19-8-29.
//

#ifndef KUNKUN_CLIENTUTILS_H
#define KUNKUN_CLIENTUTILS_H


class ClientUtils {
private:

public:

    static void login(const std::string & username, const std::string & password, bool & finished, SocketCallback callback);
};


#endif //KUNKUN_CLIENTUTILS_H
