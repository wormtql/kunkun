//
// Created by iatow on 19-8-28.
//

#ifndef HOMEWORK_NETWORKUTILS_H
#define HOMEWORK_NETWORKUTILS_H


class Client {
private:


public:
    static Client * getIns();



    int send_string(const std::string & message);


    int send_image_from_file(const std::string & filename);


    int send_image_from_pixbuf(GdkPixbuf * image);


    int send_file_from_file(const std::string & filename);
};


#endif //HOMEWORK_NETWORKUTILS_H
