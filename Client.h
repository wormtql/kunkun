//
// Created by iatow on 19-8-28.
//

#ifndef HOMEWORK_NETWORKUTILS_H
#define HOMEWORK_NETWORKUTILS_H


class Client {
private:
    int sockfd;

    static const char server_ip[20];
    static const int SERVER_PORT;
    static const int BUFSIZE;

//    int port;
    struct sockaddr_in pin;
    char * buf;


    static Client * ins;

    // for get ins
    static GMutex ins_mutex;
    // for send
    GMutex send_mutex;
    // for recv
    GMutex recv_mutex;

    Client();

//    void init();
//    int creat_socket();
//    int connect_to_server();
//    int send_msg(const std::string &);
//    void set_port(int Port);
//    int get_sockfd();
//    int string_to_int( char *s );
//    const char* get_buf();
    //void set_msg(char* Msg);

public:
    static Client * getIns();


    int initialize_net();


    int recv_msg(char * ret);


    int recv_blocked(char * ret);


    int send_string(const std::string & message);

    int send_char_arr(char * content, int size);


//    int send_image_from_file(const std::string & filename);


//    int send_image_from_pixbuf(GdkPixbuf * image);


//    int send_file_from_file(const std::string & filename);
};


#endif //HOMEWORK_NETWORKUTILS_H
