//
// Created by iatow on 19-8-29.
//

#ifndef KUNKUN_THREAD_H
#define KUNKUN_THREAD_H


class Thread {
private:
    static char buf[20000];

    static pthread_t id;

    static SocketCallback chat_recv_msg_cb;
    static SocketCallback group_recv_msg_cb;
    static SocketCallback recv_sys_msg_cb;
    static SocketCallback recv_file_cb;


    static GMutex mutex;



    static void * service_func(void *);

public:
    static SocketCallback default_cb;

    static void init();

    static void stop_recv();

    static void resume_recv();

    static void set_call_back(str type, SocketCallback callback);

    static void kill();
};


#endif //KUNKUN_THREAD_H
