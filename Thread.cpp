//
// Created by iatow on 19-8-29.
//

#include "INCLUDE.h"

//#include "Thread.h"

using std::string;

char Thread::buf[20000] = { 0 };
pthread_t Thread::id = 0;
SocketCallback Thread::default_cb = [] (char * data, int size) {};
SocketCallback Thread::chat_recv_msg_cb = [] (char * data, int size) {};
SocketCallback Thread::group_recv_msg_cb = [] (char * data, int size) {};
SocketCallback Thread::recv_sys_msg_cb = [] (char * data, int size) {};
SocketCallback Thread::recv_file_cb = [] (char * data, int size) {};
//int Thread::lock = 0;
GMutex Thread::mutex;


/*
 * 功能：接收服务器端的推送信息
 * 参数：无
 * 返回值：无
 */
void * Thread::service_func(void *) {
    printf("thread start\n");


    while (true) {

        g_mutex_lock(&mutex);

        memset(buf, 0, sizeof(char) * 20000);
        int status = Client::getIns()->recv_msg(buf);

        if (status == -1) {
            // error
            printf("error\n");
            g_mutex_unlock(&mutex);
            break;
        } else if (status == 0) {
            // no message
//            printf("no message\n");
            g_mutex_unlock(&mutex);
            continue;
        } else if (status == -2) {
            // connection break
            printf("disconnect\n");
            g_mutex_unlock(&mutex);
            break;
        }

        printf("**********************\n");
        printf("buf: %s\n", buf);
        printf("**********************\n");


//        callback(buf);

//        json retj = json::parse(buf);
//        std::string cmd = retj["command"];

        json retj;
        int jsize = Utils::parse_recv_file(buf, retj);
        std::string cmd = retj["command"];

        if (cmd == "chat_recv_msg") {
            chat_recv_msg_cb(buf, status);
        } else if (cmd == "group_recv_msg") {
            group_recv_msg_cb(buf, status);
        } else if (cmd == "recv_sys_msg") {
            recv_sys_msg_cb(buf, status);
        } else if (cmd == "recv_file") {
            recv_file_cb(buf, status);
        }

//        printf("aaa\n");
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

/*
 * 初始化接收线程
 *
 */
void Thread::init() {
//    mutex = new GMutex;
    g_mutex_init(&mutex);
    g_thread_new("worm", Thread::service_func, nullptr);
}

/*
 * 断开线程
 *
 */
void Thread::kill() {
    exit(0);
}

/*
 * 暂停接收
 *
 */
void Thread::stop_recv() {
//    while (lock == 1);
//    lock = 1;
    g_mutex_lock(&mutex);
}

/*
 *
 *
 */
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
    } else if (type == "recv_sys_msg") {
        recv_sys_msg_cb = callback;
    } else if (type == "recv_file") {
        recv_file_cb = callback;
    }

    Thread::resume_recv();
}