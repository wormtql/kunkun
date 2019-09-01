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
        FileSendCallback callback;
    };

    static void* thread_func(void *);
public:
    static void send_file(str filename, str fileid, FileSendCallback callback);
};


#endif //KUNKUN_SENDFILEUTILS_H
