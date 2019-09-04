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
#include <algorithm>
#include <ios>
#include <vector>
#include <functional>
#include <cctype>
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <mysql/mysql.h>
#include <sys/stat.h>

#include "json.hpp"


typedef std::function<void (char *, int size)> SocketCallback;
typedef std::function<void (double)> FileSendCallback;
typedef const std::string & str;


#include "DataHub.h"
#include "CssLoader.h"
#include "LoginWindow.h"
#include "Client.h"
#include "ChatPanel.h"
#include "UserPanel.h"
#include "AddFriendPanel.h"
#include "ConsolePanel.h"
#include "MainWindow.h"
#include "ClientUtils.h"
#include "Utils.h"
#include "Thread.h"
#include "SendFileUtils.h"

#define CLIENT_BUF_SIZE 256
#define MSG_SIZE 256
#define FILE_BLOCK_SIZE 256

#endif //HOMEWORK_INCLUDE_H
