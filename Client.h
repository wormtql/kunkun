//
// Created by iatow on 19-8-28.
//

#ifndef HOMEWORK_NETWORKUTILS_H
#define HOMEWORK_NETWORKUTILS_H


class Client {
private:
    int sockfd;
//    char server_ip[100] = { 0 };
    char server_ip[20] = "127.0.0.1";
    int port;
    struct sockaddr_in pin;
    char *buf;

    Client();

    void init(int port, const char * ip);
    int creat_socket();
    int connect_to_server();
    int send_msg(const std::string &);
    void set_port(int Port);
    int get_sockfd();
    int string_to_int( char *s );
    const char* get_buf();
    //void set_msg(char* Msg);

public:
    static Client * getIns();
    int initialize_net(int Port,const char *ip);
    int recv_msg(char * ret);


    int send_string(const std::string & message);


    int send_image_from_file(const std::string & filename);


    int send_image_from_pixbuf(GdkPixbuf * image);


    int send_file_from_file(const std::string & filename);
};


#endif //HOMEWORK_NETWORKUTILS_H
