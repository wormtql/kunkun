//
// Created by iatow on 19-8-29.
//

#include "INCLUDE.h"

//#include "Thread.h"

using std::string;

char Thread::buf[20000] = { 0 };
pthread_t Thread::id = 0;
SocketCallback Thread::callback = [] (const std::string & data) {};
int Thread::lock = 0;

void * Thread::service_func(void *) {
    printf("thread start\n");

    while (true) {
        int status = Client::getIns()->recv_msg(buf);

        if (status == -1) {
            break;
        } else if (status == 0) {
            continue;
        }

        lock = 1;
        callback(buf);
        lock = 0;
    }
}

//Thread* Thread::getIns() {
//    static Thread ins;
//
//    return &ins;
//}

void Thread::init() {
    g_thread_new("worm", Thread::service_func, nullptr);
}

void Thread::set_call_back(SocketCallback callback) {
    while (lock == 1);

    lock = 1;
    Thread::callback = callback;
    lock = 0;
}

void Thread::kill() {
    exit(0);
}