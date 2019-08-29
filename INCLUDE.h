//
// Created by iatow on 19-8-27.
//

#ifndef HOMEWORK_INCLUDE_H
#define HOMEWORK_INCLUDE_H

#include <gtk/gtk.h>
#include <cairo.h>

#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include <cctype>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "json.hpp"


typedef std::function<void (const std::string &)> SocketCallback;


#include "Utils.h"
#include "CssLoader.h"
#include "LoginWindow.h"
#include "Client.h"
#include "ChatPanel.h"
#include "UserPanel.h"
#include "AddFriendPanel.h"
#include "MainWindow.h"
#include "ClientUtils.h"


#endif //HOMEWORK_INCLUDE_H
