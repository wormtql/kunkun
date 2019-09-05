//
// Created by iatow on 19-8-28.
//

#include "INCLUDE.h"

#include "Client.h"


//const char Client::server_ip[20] = "127.0.0.1";
const char Client::server_ip[20] = "10.194.139.213";
const int Client::SERVER_PORT = 1234;
const int Client::BUFSIZE = 20000;
unsigned change(const char *s)
{
    unsigned tmp=0;
    unsigned res = 0;
    int cnt=0;
    for(int i=0;s[i];i++)
    {
        if(s[i]=='.')
        {
            res+=tmp;
            res*=256;
            tmp=0;
        } else{
            tmp=tmp*10+s[i]-'0';
        }
    }
    res+=tmp;
    return res;
}
GMutex Client::ins_mutex;
Client * Client::ins = nullptr;


Client::Client(){}

/****************************************************
* Description : translate char* to int
* Prameter    : None
* Return      : None
* Date        : 2019.8.28
****************************************************/
//int Client::string_to_int(char *s) {
//    int len = strlen(s);
//    int res = 0;
//    for(int i = 0; i < len; i++ ){
//        res = res*10+s[i]-'0';
//    }
//    return res;
//}
/****************************************************
* Description : init 'pin' and 'msg' and 'buf' and 'ip' and 'port
* Prameter    : None
* Return      : None
* Date        : 2019.8.28
****************************************************/
//void Client::init(){
//    strcpy(this->server_ip, ip);
//    bzero(&pin, sizeof(pin));    //init 0
//    pin.sin_family = AF_INET;
//    pin.sin_addr.s_addr = htonl(INADDR_ANY);
//    pin.sin_port = htons(SERVER_PORT);
//    buf = (char*)malloc(sizeof(char) * BUF_SIZE);
//    bzero(buf, sizeof(char) * BUF_SIZE);
//}
/****************************************************
 * Description : create socket with AF_INET and TCP
 * Prameter    : None
 * Return      : int : 1 success  0 failed
 * Date        : 2019.8.28
 ****************************************************/
//int Client::creat_socket() {
//    if((sockfd = socket(AF_INET,SOCK_STREAM, 0)) == -1){
//        perror("Failed to create socket.\n");
//
//        return 0;
//    }
//    auto flags = fcntl(sockfd, F_GETFL, 0);
//    fcntl(sockfd, flags & ~O_NONBLOCK);
//    return 1;
//}
/****************************************************
 * Description : connect to server
 * Prameter    : None
 * Return      : int : 1 success  0 failed
 * Date        : 2019.8.28
 ****************************************************/
//int Client::connect_to_server() {
//    if(connect(sockfd,(struct sockaddr *)&pin, sizeof(pin)) == -1){
//        perror("Failed connect to server.\n");
//        return 0;
//    }
//    return 1;
//}
/****************************************************
 * Description : send message to server
 * Prameter    : None
 * Return      : int : 1 success  0 failed
 * Date        : 2019.8.28
 ****************************************************/
//int Client::send_msg(const std::string & msg) {
//    if(send(sockfd,msg.c_str(),msg.length(),0) == -1){
//        perror("Failed to send message to server.\n");
//        return 0;
//    }
//    printf("Send \" %s \" to server successfully.\n",msg.c_str());
//    return 1;
//}


/*
 * function: 接收消息，若没有消息也不阻塞
 *
 */
int Client::recv_msg(char * ret) {

    memset(buf, 0, sizeof(char) * BUFSIZE);
    int status = (int)recv(sockfd, buf, BUFSIZE, 0);

    if(status == -1){
        if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
            // return nothing;
            return 0;
        } else {
            // return error
            return -1;
        }
    } else if (status == 0) {
        // connection break

        close(this->sockfd);

        return -2;
    }

    printf("recv: %s\n", buf);
    memcpy(ret, buf, status);

    return status;
}


/**********************************************
 * Description: 接收消息，若没有消息则阻塞
 *
 **********************************************/
int Client::recv_blocked(char *ret) {
    while (true) {
        memset(buf, 0, sizeof(char) * BUFSIZE);
        int status = (int)recv(sockfd, buf, BUFSIZE, 0);

        if (status == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
                continue;
            } else {
                // return error
                return -1;
            }
        } else if (status == 0) {
            close(this->sockfd);

            return -2;
        } else {

            strcpy(ret, buf);
            printf("recv block: %s\n", buf);

            return status;
        }
    }
}

//void Client::set_port(int Port) {
//    port = Port;
//}

//const char* Client::get_buf() {
//    return (const char*)buf;
//}

//int Client::get_sockfd() {
//    return sockfd;
//}

/****************************************************
 * Description : singleton Instance mode
 * Prameter    : None
 * Return      : a static client class
 * Date        : 2019.8.29
 ****************************************************/
Client* Client::getIns() {
//    static Client ins;
//    return &ins;

    if (ins == nullptr)
    {
        g_mutex_lock(&ins_mutex);
        if (ins == nullptr)
        {
            ins = new Client();
        }
        g_mutex_unlock(&ins_mutex);
    }

    return ins;

}
/****************************************************
 * Description : send message to server
 * Prameter    : None
 * Return      : int : 1 success  0 failed
 * Date        : 2019.8.29
 ****************************************************/
int Client::send_string(const std::string &message) {

    // only one thread can send

    g_mutex_lock(&send_mutex);

    int status = (int)send(sockfd, message.c_str(), sizeof(char) * message.size(), 0);

    g_mutex_unlock(&send_mutex);

    return status;
}
/****************************************************
 * Description : initialize the net(include creat socket,connect to server and init member in class
 * Prameter    : port , server ip
 * Return      : int : 1 success  0 failed
 * Date        : 2019.8.29
 ****************************************************/
int Client::initialize_net() {
    buf = new char[BUFSIZE];
    memset(buf, 0, sizeof(char) * BUFSIZE);

    sockaddr_in pin_addr = { 0 };

    memset(&pin_addr, 0, sizeof(pin_addr));

    pin_addr.sin_family = AF_INET;
    pin_addr.sin_port = htons(SERVER_PORT);
//    pin_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    char buf[100] = { 0 };
    std::ifstream file("../ip");
    file.getline(buf, 100);

    std::cout << buf << std::endl;

//    pin_addr.sin_addr.s_addr = htonl(change(server_ip));
    pin_addr.sin_addr.s_addr = htonl(change(buf));

    this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->sockfd == -1) {
        printf("error creating socket\n");
        return -1;
    }

    if (connect(sockfd, (sockaddr *)&pin_addr, sizeof(pin_addr)) == -1) {
        printf("connect error! consider start a backend program\n");
        return -1;
    }

    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

    printf("init socket successful\n");

    return 1;
}
/****************************************************
 * Description : send file to server
 * Prameter    : filename
 * Return      : int : 1 success  0 failed
 * Date        : 2019.8.29
 * Ps          : without test!!!!
 ****************************************************/

//int Client::send_file_from_file(const std::string &filename) {
//    FILE *fp = fopen(filename.c_str(),"r");
//    if(fp == NULL){
//        perror("Failed to open file.");
//        return 0;
//    }
//    int block_length;
//    char file_buf[FILE_BLOCK_SIZE];
//    bzero(file_buf,sizeof(file_buf));
//    while((block_length = fread(file_buf, sizeof(char),BUF_SIZE,fp)) > 0){
//        if(Client::getIns()->send_msg(file_buf) == 0){
//            perror("Failed to send file block.");
//            return 0;
//        }
//        printf("Successfully send %dByte to server.  --File : %s-- ",block_length,filename.c_str());
//        bzero(file_buf,sizeof(file_buf));
//    }
//    printf("Finished transfer File : %s.",filename.c_str());
//    return 1;
//}

int Client::send_char_arr(char *content, int size) {
    g_mutex_lock(&send_mutex);

    int status = (int)send(sockfd, content, size, 0);

    g_mutex_unlock(&send_mutex);

    return status;
}