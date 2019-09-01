//
// Created by iatow on 19-9-1.
//

#include "INCLUDE.h"

#include "SendFileUtils.h"

using std::string;
using std::ifstream;

void* SendFileUtils::thread_func(void * user_data) {
    auto data = (Data *)user_data;


    ifstream file(data->filename);

    file.seekg(0, std::ios::end);

    double total_byte = ceil(file.tellg() / 1024.0);

    file.seekg(0);


    char * buf = new char[1024];

    int sent_byte = 0;
    while (true)
    {
        memset(buf, 0, sizeof(char) * 1024);
        file.read(buf, 1024);

//        printf("worm\n");


        bool eof = file.eof();
        ClientUtils::chat_send_file(data->fileid, eof, buf);

        sent_byte++;

        data->callback(1.0 * sent_byte / total_byte);
//        sleep(500);

        if (eof) {
            break;
        }
    }

    file.close();

    printf("send_file_complete\n");

//    g_thread_exit(new int(0));
}

void SendFileUtils::send_file(const std::string &filename, str fileid, FileSendCallback callback) {


    Data * data = new Data;
    data->fileid = fileid;
    data->filename = filename;
    data->callback = callback;




//    g_thread_new("send_file_thread", SendFileUtils::thread_func, data);
    g_idle_add([] (gpointer user_data) -> gboolean {
        thread_func(user_data);

        return false;
    }, data);
}