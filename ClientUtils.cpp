//
// Created by iatow on 19-8-29.
//

#include "INCLUDE.h"
#include "ClientUtils.h"

// 阻塞的 login
json ClientUtils::login_blocked(const std::string &username, const std::string &password) {
    // 阻塞接收

    Thread::stop_recv();

    json j;
    j["command"] = "login";
    j["username"] = username;
    j["password"] = password;

    Client::getIns()->send_string(j.dump());

    char buf[CLIENT_BUF_SIZE];
    Client::getIns()->recv_blocked(buf);


    Thread::resume_recv();


    return json::parse(buf);
}

// 阻塞的 signup
json ClientUtils::signup_blocked(const std::string &username, const std::string &password) {
    // 阻塞接收

    Thread::stop_recv();

    json j;
    j["command"] = "signup";
    j["username"] = username;
    j["password"] = password;

    Client::getIns()->send_string(j.dump());

    char buf[CLIENT_BUF_SIZE];
    Client::getIns()->recv_blocked(buf);


    Thread::resume_recv();


    return json::parse(buf);
}


// 阻塞的 create group
json ClientUtils::create_group_blocked(const std::string &who, const std::string &group_name) {
    // 阻塞接收

    Thread::stop_recv();

    json j;
    j["command"] = "create_group";
    j["who"] = who;
    j["group_name"] = group_name;

    Client::getIns()->send_string(j.dump());

    char buf[CLIENT_BUF_SIZE];
    Client::getIns()->recv_blocked(buf);


    Thread::resume_recv();


    return json::parse(buf);
}

// 阻塞的 rename group
json ClientUtils::rename_group_blocked(int group_id, const std::string &new_name, const std::string & who) {
    // 阻塞接收

    Thread::stop_recv();

    json j;
    j["command"] = "rename_group";
    j["who"] = who;
    j["group_id"] = group_id;
    j["new_name"] = new_name;

    Client::getIns()->send_string(j.dump());

    char buf[CLIENT_BUF_SIZE];
    Client::getIns()->recv_blocked(buf);


    Thread::resume_recv();


    return json::parse(buf);
}

// 阻塞的 send ad friend request
json ClientUtils::send_add_friend_request_blocked(const std::string &sender, const std::string & recipient) {
    // 阻塞接收

    Thread::stop_recv();

    json j;
    j["command"] = "send_add_friend_request";
    j["from"] = sender;
    j["to"] = recipient;

    Client::getIns()->send_string(j.dump());

    char buf[CLIENT_BUF_SIZE];
    Client::getIns()->recv_blocked(buf);


    Thread::resume_recv();


    return json::parse(buf);
}
// 阻塞的 send ad friend result
json ClientUtils::send_add_friend_result_blocked(const std::string & applier, const std::string & recipient, bool result) {
    // 阻塞接收
    Thread::stop_recv();

    json j;
    j["command"] = "send_add_friend_result";
    j["from"] = applier;
    j["to"] = recipient;
    j["accept"] = result;

    Client::getIns()->send_string(j.dump());

    char buf[CLIENT_BUF_SIZE];
    Client::getIns()->recv_blocked(buf);


    Thread::resume_recv();


    return json::parse(buf);
}

// 阻塞的 send chat msg
json ClientUtils::chat_send_msg_blocked(const std::string & sender, const std::string & recipient, const std::string & msg) {
    // 阻塞接收
    Thread::stop_recv();

    json j;
    j["command"] = "chat_send_msg";
    j["from"] = sender;
    j["to"] = recipient;
    j["content"] = msg;

    Client::getIns()->send_string(j.dump());

    char buf[CLIENT_BUF_SIZE];
    Client::getIns()->recv_blocked(buf);


    Thread::resume_recv();


    return json::parse(buf);
}

// 阻塞的 group send msg
json ClientUtils::group_send_msg_blocked(const std::string & sender, const std::string & group_id, const std::string & msg) {
    // 阻塞接收
    Thread::stop_recv();

    json j;
    j["command"] = "group_send_msg";
    j["from"] = sender;
    j["group_id"] = group_id;
    j["content"] = msg;

    Client::getIns()->send_string(j.dump());

    char buf[CLIENT_BUF_SIZE];
    Client::getIns()->recv_blocked(buf);


    Thread::resume_recv();


    return json::parse(buf);
}

// 阻塞的 send file begin
json ClientUtils::chat_send_file_begin_blocked(const std::string & sender, const std::string & recipient,const std::string & filename) {
    // 阻塞接收
    Thread::stop_recv();

    json j;
    j["command"] = "chat_send_file_begin";
    j["filename"] = filename;
    j["from"] = sender;
    j["to"] = recipient;

    Client::getIns()->send_string(j.dump());

    char buf[CLIENT_BUF_SIZE];
    Client::getIns()->recv_blocked(buf);


    Thread::resume_recv();


    return json::parse(buf);
}

// 阻塞的 chat send file
json ClientUtils::chat_send_file_blocked(const std::string & file_id, const std::string & file_content, bool eof) {
    // 阻塞接收
    Thread::stop_recv();

    json j;
    j["command"] = "chat_send_file";
    j["fileid"] = file_id;
    j["eof"] = eof;
    j["content"] = file_content;

    Client::getIns()->send_string(j.dump());

    char buf[CLIENT_BUF_SIZE];
    Client::getIns()->recv_blocked(buf);


    Thread::resume_recv();


    return json::parse(buf);
}






