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
GMutex Thread::mutex;

void * Thread::service_func(void *) {
    printf("thread start\n");


    int iter = 0;
    while (true) {
//        printf("%d\n", iter++);

        // require lock
//        while (lock == 1) {
//            printf("%d\n", iter++);
//        }
//        lock = 1;
        g_mutex_lock(&mutex);

        int status = Client::getIns()->recv_msg(buf);

        if (status == -1) {
            // error
//            lock = 0;
            g_mutex_unlock(&mutex);
            break;
        } else if (status == 0) {
            // no message
//            lock = 0;
            g_mutex_unlock(&mutex);
            continue;
        } else if (status == -2) {
            // connection break
//            lock = 0;
            g_mutex_unlock(&mutex);
            break;
        }

        printf("buf: %s\n", buf);


//        callback(buf);

        // release lock
//        lock = 0;
        g_mutex_unlock(&mutex);
    }
}

//Thread* Thread::getIns() {
//    static Thread ins;
//
//    return &ins;
//}

void Thread::init() {
//    mutex = new GMutex;
    g_mutex_init(&mutex);
    g_thread_new("worm", Thread::service_func, nullptr);
}

//void Thread::set_call_back(SocketCallback callback) {
//    while (lock == 1);
//
//    lock = 1;
//    Thread::callback = callback;
//    lock = 0;
//}

void Thread::kill() {
    exit(0);
}

void Thread::stop_recv() {
//    while (lock == 1);
//    lock = 1;
    g_mutex_lock(&mutex);
}

void Thread::resume_recv() {
//    lock = 0;
    g_mutex_unlock(&mutex);
}