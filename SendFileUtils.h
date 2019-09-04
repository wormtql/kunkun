//
// Created by iatow on 19-9-1.
//

#ifndef KUNKUN_SENDFILEUTILS_H
#define KUNKUN_SENDFILEUTILS_H


class SendFileUtils {
private:

    struct Data {
        std::string filename;
        std::string fileid;
        std::string from;
        std::string to;
        FileSendCallback callback;
    };

//    static void* thread_func(void *);
public:
    static void chat_send_file(str from, str to, str filename, str fileid, FileSendCallback callback);

    static void group_send_file(str from, str group_id, str filename, str fileid, FileSendCallback callback);
};


#endif //KUNKUN_SENDFILEUTILS_H
