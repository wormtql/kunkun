//
// Created by iatow on 19-8-28.
//

#include "INCLUDE.h"

#include "Client.h"

Client::Client(){}

/****************************************************
* Description : translate char* to int
* Prameter    : None
* Return      : None
* Date        : 2019.8.28
****************************************************/
int Client::string_to_int(char *s) {
    int len = strlen(s);
    int res = 0;
    for(int i = 0; i < len; i++ ){
        res = res*10+s[i]-'0';
    }
    return res;
}
/****************************************************
* Description : init 'pin' and 'msg' and 'buf' and 'ip' and 'port
* Prameter    : None
* Return      : None
* Date        : 2019.8.28
****************************************************/
void Client::init(int port, const char * ip){
    strcpy(this->server_ip, ip);
    bzero(&pin,sizeof(pin));    //init 0
    pin.sin_family = AF_INET;
    pin.sin_addr.s_addr = INADDR_ANY;
    pin.sin_port = htons(port);
    buf = (char*)malloc(sizeof(char) * BUF_SIZE);
}
/****************************************************
 * Description : create socket with AF_INET and TCP
 * Prameter    : None
 * Return      : int : 1 success  0 failed
 * Date        : 2019.8.28
 ****************************************************/
int Client::creat_socket() {
    if(sockfd = socket(AF_INET,SOCK_STREAM,0) == -1){
        perror("Failed to create socket.\n");
        return 0;
    }
    return 1;
}
/****************************************************
 * Description : connect to server
 * Prameter    : None
 * Return      : int : 1 success  0 failed
 * Date        : 2019.8.28
 ****************************************************/
int Client::connect_to_server() {
    if(connect(sockfd,(struct sockaddr *)&pin, sizeof(pin)) == -1){
        perror("Failed connect to server.\n");
        return 0;
    }
    return 1;
}
/****************************************************
 * Description : send message to server
 * Prameter    : None
 * Return      : int : 1 success  0 failed
 * Date        : 2019.8.28
 ****************************************************/
int Client::send_msg(const std::string & msg) {
    if(send(sockfd,msg.c_str(),msg.length(),0) == -1){
        perror("Failed to send message to server.\n");
        return 0;
    }
    printf("Send \" %s \" to server successfully.\n",msg.c_str());
    return 1;
}
/****************************************************
 * Description : receive message from server
 * Prameter    : None
 * Return      : int : 1 success  0 failed
 * Date        : 2019.8.28
 ****************************************************/
int Client::recv_msg(char * ret) {
    int status = recv(sockfd, buf, BUF_SIZE, 0);
    if(status == -1){
        perror("Failed to receive message from server.\n");
        return 0;
    }
    strcpy(ret, buf);
    return 1;
}

void Client::set_port(int Port) {
    port = Port;
}

const char* Client::get_buf() {
    return (const char*)buf;
}

int Client::get_sockfd() {
    return sockfd;
}

/****************************************************
 * Description : singleton Instance mode
 * Prameter    : None
 * Return      : a static client class
 * Date        : 2019.8.29
 ****************************************************/
Client* Client::getIns() {
    static Client ins;

    return &ins;
}
/****************************************************
 * Description : send message to server
 * Prameter    : None
 * Return      : int : 1 success  0 failed
 * Date        : 2019.8.29
 ****************************************************/
int Client::send_string(const std::string &message) {
    if(Client::getIns()->send_msg(message) == 0){
        return 0;
    }
    return 1;
}
/****************************************************
 * Description : initialize the net(include creat socket,connect to server and init member in class
 * Prameter    : port , server ip
 * Return      : int : 1 success  0 failed
 * Date        : 2019.8.29
 ****************************************************/
int Client::initialize_net(int Port, const char *ip) {
    Client::getIns()->init(Port,ip);
    if(Client::getIns()->creat_socket() == 0)return 0;
    if(Client::getIns()->connect_to_server() == 0)return 0;
    return 1;
}