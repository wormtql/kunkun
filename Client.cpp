//
// Created by iatow on 19-8-28.
//

#include "INCLUDE.h"

#include "Client.h"

Client* Client::getIns() {
    static Client ins;

    return &ins;
}