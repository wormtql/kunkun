//
// Created by iatow on 19-8-29.
//

#include "INCLUDE.h"

//#include "Thread.h"

using std::string;

char Thread::buf[20000] = { 0 };
pthread_t Thread::id = 0;
SocketCallback Thread::default_cb = [] (const std::string & data) {};
SocketCallback Thread::chat_recv_msg_cb = [] (const std::string & data) {};
SocketCallback Thread::group_recv_msg_cb = [] (const std::string & data) {};
SocketCallback Thread::recv_sys_msg_cb = [] (const std::string & data) {};
SocketCallback Thread::recv_file_cb = [] (const std::string & data) {};
//int Thread::lock = 0;
GMutex Thread::mutex;

void * Thread::service_func(void *) {
    printf("thread start\n");


    while (true) {

        g_mutex_lock(&mutex);

        int status = Client::getIns()->recv_msg(buf);

        if (status == -1) {
            // error

            g_mutex_unlock(&mutex);
            break;
        } else if (status == 0) {
            // no message

            g_mutex_unlock(&mutex);
            continue;
        } else if (status == -2) {
            // connection break

            g_mutex_unlock(&mutex);
            break;
        }

        printf("buf: %s\n", buf);


//        callback(buf);

        json retj = json::parse(buf);
        std::string cmd = retj["command"];

        if (cmd == "chat_recv_msg") {
            chat_recv_msg_cb(buf);
        } else if (cmd == "group_recv_msg") {
            group_recv_msg_cb(buf);
        } else if (cmd == "recv_sys_msg") {
            recv_sys_msg_cb(buf);
        } else if (cmd == "recv_file") {
            recv_file_cb(buf);
        }


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

void Thread::set_call_back(const std::string &type, SocketCallback callback) {
    Thread::stop_recv();

    if (type == "chat_recv_msg") {
        chat_recv_msg_cb = callback;
    } else if (type == "group_recv_msg") {
        group_recv_msg_cb = callback;
    } else if (type == "recv_add_friend_result") {
        recv_sys_msg_cb = callback;
    } else if (type == "recv_file") {
        recv_file_cb = callback;
    }

    Thread::resume_recv();
}