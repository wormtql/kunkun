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

//    Thread() = default;

public:

    static void init();

    static void * service_func(void *);

    static void set_call_back(SocketCallback callback);

    static void kill();
};


#endif //KUNKUN_THREAD_H
