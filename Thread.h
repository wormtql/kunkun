//
// Created by iatow on 19-8-29.
//

#ifndef KUNKUN_THREAD_H
#define KUNKUN_THREAD_H


class Thread {
private:
    static char buf[20000];

    static pthread_t id;

    static SocketCallback callback;
    static int lock;

    static int stop;

    static GMutex mutex;

//    Thread() = default;

    static void * service_func(void *);

public:

    static void init();

    static void stop_recv();

    static void resume_recv();

//    static void set_call_back(SocketCallback callback);

    static void kill();
};


#endif //KUNKUN_THREAD_H
