//
// Created by iatow on 19-9-1.
//

#include "INCLUDE.h"

#include "SendFileUtils.h"

using std::string;
using std::ifstream;

//void* SendFileUtils::thread_func(void * user_data) {
//    auto data = (Data *)user_data;
//
//
//    ifstream file(data->filename);
//
//    file.seekg(0, std::ios::end);
//
//    double total_byte = ceil(file.tellg() / 1024.0);
//
//    file.seekg(0);
//
//
//    char * buf = new char[1024];
//
//    int sent_byte = 0;
//    while (true)
//    {
//        memset(buf, 0, sizeof(char) * 1024);
//        file.read(buf, 1024);
//
////        printf("worm\n");
//
//
//        bool eof = file.eof();
//        ClientUtils::chat_send_file(data->fileid, eof, buf);
//
//        sent_byte++;
//
//        data->callback(1.0 * sent_byte / total_byte);
////        sleep(500);
//
//        if (eof) {
//            break;
//        }
//    }
//
//    file.close();
//
//    printf("send_file_complete\n");
//
////    g_thread_exit(new int(0));
//}

//void SendFileUtils::send_file(const std::string &filename, str fileid, FileSendCallback callback) {
//
//
//    Data * data = new Data;
//    data->fileid = fileid;
//    data->filename = filename;
//    data->callback = callback;
//
//
//
//
////    g_thread_new("send_file_thread", SendFileUtils::thread_func, data);
//    g_idle_add([] (gpointer user_data) -> gboolean {
//        thread_func(user_data);
//
//        return false;
//    }, data);
//}

// filename is full path
void SendFileUtils::chat_send_file(const std::string &from, const std::string &to, const std::string &filename,
                                   const std::string &fileid, FileSendCallback callback) {
    Data * data = new Data;

    data->fileid = fileid;
    data->callback = callback;
    data->from = from;
    data->to = to;
    data->filename = filename;
//
//    g_thread_new("send_file_thread", [] (void * user_data) -> void * {
//
//        auto d = (Data *)user_data;
//
//        ifstream file(d->filename);
//        file.seekg(0, std::ios::end);
//        int total_byte = (int)file.tellg();
//        file.seekg(0);
//
//        char * buf = new char[1024];
//
//        int sent_byte = 0;
//        while (true)
//        {
//            memset(buf, 0, sizeof(char) * 1024);
//            file.read(buf, 1024);
//
//            bool eof = file.eof();
//
//            int read_byte = eof ? total_byte % 1024 : 1024;
//
//            sent_byte += read_byte;
//
//            ClientUtils::chat_send_file(d->fileid, d->from, d->to, eof, buf, read_byte);
//
//            d->callback(1.0 * sent_byte / total_byte);
//
//            if (eof) {
//                break;
//            }
//        }
//
//        file.close();
//
//        delete[] buf;
//
//        printf("send_file_complete\n");
//
//    }, data);


    g_idle_add([] (gpointer user_data) -> gboolean {

        auto d = (Data *)user_data;

        ifstream file(d->filename);
        file.seekg(0, std::ios::end);
        int total_byte = (int)file.tellg();
        file.seekg(0);

        char * buf = new char[1024];

        int sent_byte = 0;
        while (true)
        {
            memset(buf, 0, sizeof(char) * 1024);
            file.read(buf, 1024);

            bool eof = file.eof();

            int read_byte = eof ? total_byte % 1024 : 1024;

            sent_byte += read_byte;

            ClientUtils::chat_send_file(d->fileid, d->from, d->to, eof, buf, read_byte);

            d->callback(1.0 * sent_byte / total_byte);

            if (eof) {
                break;
            }
        }

        file.close();

        delete[] buf;

        printf("send_file_complete\n");

        return false;


    }, data);
}

void SendFileUtils::group_send_file(const std::string &from, const std::string &group_id, str filename, const std::string &fileid,
                                    FileSendCallback callback) {
    Data * data = new Data;

    data->fileid = fileid;
    data->callback = callback;
    data->from = from;
    data->to = group_id;
    data->filename = filename;


    g_idle_add([] (gpointer user_data) -> gboolean {
        auto d = (Data *)user_data;

        ifstream file(d->filename);
        file.seekg(0, std::ios::end);
        int total_byte = (int)file.tellg();
        file.seekg(0);

        char * buf = new char[1024];

        int sent_byte = 0;
        while (true)
        {
            memset(buf, 0, sizeof(char) * 1024);
            file.read(buf, 1024);



            bool eof = file.eof();

            int read_byte = eof ? total_byte % 1024 : 1024;

            sent_byte += read_byte;

//            ClientUtils::chat_send_file(d->fileid, d->from, d->to, eof, buf, read_byte);
            ClientUtils::group_send_file(d->fileid, d->from, d->to, eof, buf, read_byte);

            d->callback(1.0 * sent_byte / total_byte);

            if (eof) {
                break;
            }
        }

        file.close();

        delete[] buf;

        printf("send_file_complete\n");

        return false;
    }, data);

//    g_thread_new("send_file_thread", [] (void * user_data) -> void * {
//
//
//
//    }, data);
}