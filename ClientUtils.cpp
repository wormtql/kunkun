//
// Created by iatow on 19-8-29.
//

#include "INCLUDE.h"
#include "ClientUtils.h"


char ClientUtils::buf[20000] = { 0 };

// 阻塞的 login
json ClientUtils::login_blocked(const std::string &username, const std::string &password) {
    // 阻塞接收

    Thread::stop_recv();

    json j;
    j["command"] = "login";
    j["username"] = username;
    j["password"] = password;

    Client::getIns()->send_string(j.dump());


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

    Client::getIns()->recv_blocked(buf);


    Thread::resume_recv();


    return json::parse(buf);
}

json ClientUtils::get_list_user_like_blocked(const std::string &query_str) {
    Thread::stop_recv();

    json j;
    j["command"] = "list_user_like";
    j["query_str"] = query_str;

    Client::getIns()->send_string(j.dump());

    Client::getIns()->recv_blocked(buf);

    Thread::resume_recv();

    json retj = json::parse(buf);
    if (retj["debug"])
    {
        return R"(["user1", "user2"])"_json;
    }
    else
    {
        return retj["list"];
    }
}

json ClientUtils::get_list_group_like_blocked(const std::string &query_str) {
    Thread::stop_recv();

    json j;
    j["command"] = "list_group_like";
    j["query_str"] = query_str;

    Client::getIns()->send_string(j.dump());

    Client::getIns()->recv_blocked(buf);

    Thread::resume_recv();

    json retj = json::parse(buf);
    if (retj["debug"])
    {
        return R"([{"name": "group1", "id": "id1"},
                   {"name": "group2", "id": "id2"}])"_json;
    }
    else
    {
        return retj["list"];
    }
}

json ClientUtils::get_friends(const std::string &who)
{
    Thread::stop_recv();

    json j;
    j["command"] = "get_friends";
    j["who"] = who;

    Client::getIns()->send_string(j.dump());

    Client::getIns()->recv_blocked(buf);

    Thread::resume_recv();

    printf("server: %s\n", buf);

    json retj = json::parse(buf);
    if (retj["debug"])
    {
        return R"([{"name": "user1", "tag": "my_friend"}, {"name": "user2", "tag": "blacklist"}, {"name": "user3", "tag": "special"}])"_json;
    }
    else
    {
        return retj["list"];
    }
}

json ClientUtils::get_groups(const std::string &who) {
    Thread::stop_recv();

    json j;
    j["command"] = "get_groups";
    j["who"] = who;

    Client::getIns()->send_string(j.dump());

    Client::getIns()->recv_blocked(buf);

    Thread::resume_recv();

    json retj = json::parse(buf);
    if (retj["debug"])
    {
        return R"([{"name": "group1", "id": "id1"}, {"name": "group2", "id": "id2"}])"_json;
    }
    else
    {
        return retj["list"];
    }
}

//void ClientUtils::send_text_to_friend(const std::string &from, const std::string &to, const std::string &content) {
//    Thread::stop_recv();
//
//    json j;
//    j["command"] = "send_text_to_friend";
//    j["from"] = from;
//    j["to"] = to;
//    j["content"] = content;
//
//    Client::getIns()->send_string(j.dump());
//
//    Thread::resume_recv();
//}

//void ClientUtils::send_text_to_group(const std::string &from, const std::string &group_id,
//                                     const std::string &content) {
//    Thread::stop_recv();
//
//    json j;
//    j["command"] = "send_text_to_group";
//    j["from"] = from;
//    j["to"] = group_id;
//    j["content"] = content;
//
//    Client::getIns()->send_string(j.dump());
//
//    Thread::resume_recv();
//}

json ClientUtils::get_chat_friend_history(const std::string &from, const std::string &user) {
    Thread::stop_recv();

    json j;
    j["command"] = "get_chat_friend_history";
    j["from"] = from;
    j["user"] = user;

    Client::getIns()->send_string(j.dump());

    Client::getIns()->recv_blocked(buf);

    Thread::resume_recv();

    json retj = json::parse(buf);
    if (retj["debug"])
    {
        return R"([{"name": "user1", "type": "text", "content": "hello"},
                   {"name": "", "type": "file", "file_name": "file_name", "file_id": "file_id1"}])"_json;
    }
    else
    {
        return retj["list"];
    }
}

json ClientUtils::get_chat_group_history(const std::string &from, const std::string &group_id) {
    Thread::stop_recv();

    json j;
    j["command"] = "get_chat_group_history";
    j["from"] = from;
    j["group_id"] = group_id;

    Client::getIns()->send_string(j.dump());

    Client::getIns()->recv_blocked(buf);

    Thread::resume_recv();

    json retj = json::parse(buf);
    if (retj["debug"])
    {
        return R"([{"name": "user1", "type": "text", "content": "hello"},
                   {"name": "user2", "type": "file", "file_name": "xxx.docx", "file_id": "file_id"},
                   {"name": "", "type": "text", "content": "worm"}])"_json;
    }
    else
    {
        return retj["list"];
    }
}

json ClientUtils::alter_user_info(const std::string &who, const std::string &field, const std::string & new_val) {
    Thread::stop_recv();

    json j;
    j["command"] = "alter_user_info";
    j["who"] = who;
    j["field"] = field;
    j["new_val"] = new_val;

    Client::getIns()->send_string(j.dump());

    Client::getIns()->recv_blocked(buf);

    Thread::resume_recv();



    return json::parse(buf);
}

std::string ClientUtils::get_user_info(const std::string &who, const std::string &field) {
    Thread::stop_recv();

    json j;
    j["command"] = "get_user_info";
    j["who"] = who;
    j["field"] = field;

    Client::getIns()->send_string(j.dump());

    Client::getIns()->recv_blocked(buf);

    Thread::resume_recv();

    json retj = json::parse(buf);

    printf("server: %s\n", buf);

    if (retj["debug"])
    {
        return "male";
    }
    else
    {
        return retj["val"];
    }
}

//void ClientUtils::request_add_friend(const std::string &from, const std::string &to) {
//
//}


// 阻塞的 create group
//json ClientUtils::create_group(const std::string &who, const std::string &group_name) {
//    // 阻塞接收
//
//    Thread::stop_recv();
//
//    json j;
//    j["command"] = "create_group";
//    j["who"] = who;
//    j["group_name"] = group_name;
//
//    Client::getIns()->send_string(j.dump());
//
//    char buf[CLIENT_BUF_SIZE];
//    Client::getIns()->recv_blocked(buf);
//
//
//    Thread::resume_recv();
//
//
//    return json::parse(buf);
//}

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
json ClientUtils::send_add_friend_request(const std::string &from, const std::string & to) {
    // 阻塞接收

    Thread::stop_recv();

    json j;
    j["command"] = "send_add_friend_request";
    j["from"] = from;
    j["to"] = to;

    Client::getIns()->send_string(j.dump());

    memset(buf, 0, sizeof(buf));
    Client::getIns()->recv_blocked(buf);

    printf("server: %s\n", buf);

    Thread::resume_recv();

    return json::parse(buf);
}

void ClientUtils::request_join_group(const std::string &from, const std::string &group_id) {
    Thread::stop_recv();

    json j;
    j["command"] = "send_join_group_request";
    j["from"] = from;
    j["group_id"] = group_id;

    Client::getIns()->send_string(j.dump());

    Client::getIns()->recv_blocked(buf);

    Thread::resume_recv();
}

void ClientUtils::send_file_begin(const std::string &filename,
        SocketCallback callback) {
    Thread::stop_recv();

    json j;
    j["command"] = "send_file_begin";
//    j["from"] = from;
//    j["to"] = to;
    j["filename"] = filename;

    Client::getIns()->send_string(j.dump());

    Client::getIns()->recv_blocked(buf);

    Thread::resume_recv();

    callback(buf, -1);
}

void ClientUtils::chat_send_file(const std::string &file_id, str from, str to, bool eof, char * content, int size) {
    Thread::stop_recv();

    json j;
    j["command"] = "chat_send_file";
    j["eof"] = eof;
    j["from"] = from;
    j["to"] = to;
    j["fileid"] = file_id;
//    j["size"] = size;
//    j["content"] = content;

    std::string d = j.dump();

    char sbuf[2048] = { 0 };
    sprintf(sbuf, "%s", d.c_str());
    memcpy(sbuf + d.size(), content, size);

//    Client::getIns()->send_string(j.dump());

    Client::getIns()->send_char_arr(sbuf, size + d.size());

    Client::getIns()->recv_blocked(buf);

    Thread::resume_recv();
}

void ClientUtils::group_send_file(str fileid, const std::string &from, const std::string &group_id, bool eof, char *content,
                                  int size) {
    Thread::stop_recv();

    json j;
    j["command"] = "group_send_file";
    j["eof"] = eof;
    j["from"] = from;
    j["group_id"] = group_id;
    j["fileid"] = fileid;

    std::string d = j.dump();

    char sbuf[2048] = { 0 };
    sprintf(sbuf, "%s", d.c_str());
    memcpy(sbuf + d.size(), content, size);

    Client::getIns()->send_char_arr(sbuf, size + d.size());

    printf("file: %s\n", sbuf);

    Client::getIns()->recv_blocked(buf);

    Thread::resume_recv();
}
//
//void ClientUtils::group_send_file_begin(const std::string &from, const std::string &group_id,
//                                        const std::string &filename, SocketCallback callback) {
//    Thread::stop_recv();
//
//    json j;
//    j["command"] = "group_send_file_begin";
//    j["from"] = from;
//    j["group_id"] = group_id;
//    j["filename"] = filename;
//
//    Client::getIns()->send_string(j.dump());
//
//    Client::getIns()->recv_blocked(buf);
//
//    Thread::resume_recv();
//
//    callback(buf);
//}

void ClientUtils::send_me_a_file(const std::string &fileid) {
    Thread::stop_recv();

    json j;
    j["command"] = "send_me_a_file";
    j["fileid"] = fileid;
    j["from"] = DataHub::getIns()->username;

    Client::getIns()->send_string(j.dump());

    Client::getIns()->recv_blocked(buf);


    json retj = json::parse(buf);

    std::string filename = retj["filename"];
    std::string path = "files/" + fileid + "_" + filename;

    FILE * fp = fopen(path.c_str(), "wb");
    fclose(fp);


    Thread::resume_recv();
}

json ClientUtils::list_friend_request(const std::string &username) {
    Thread::stop_recv();

    json j;
    j["command"] = "list_friend_request";
    j["username"] = username;

    Client::getIns()->send_string(j.dump());

    Client::getIns()->recv_blocked(buf);

    Thread::resume_recv();

    json retj = json::parse(buf);

    if (retj["debug"]) {
        return R"(["user1", "user2"])"_json;
    } else {
        return retj["list"];
    }
}

json ClientUtils::list_group_invitation(const std::string &username) {
    Thread::stop_recv();

    json j;
    j["command"] = "list_group_invitation";
    j["username"] = username;

    Client::getIns()->send_string(j.dump());

    Client::getIns()->recv_blocked(buf);

    Thread::resume_recv();

    json retj = json::parse(buf);

    if (retj["debug"]) {
        return R"([
                    {
                "name": "groupname1",
                "id": "groupid1",
                "boss": "[username]"
            },
            {
                "name": "groupname2",
                "id": "groupid2",
                "boss": "[username]"
            }])"_json;
    } else {
        return retj["list"];
    }
}

json ClientUtils::list_group_request(const std::string &username) {
    Thread::stop_recv();

    json j;
    j["command"] = "list_group_request";
    j["username"] = username;

    Client::getIns()->send_string(j.dump());

    Client::getIns()->recv_blocked(buf);

    Thread::resume_recv();

    json retj = json::parse(buf);

    printf("list_group_req return: %s\n", buf);

    if (retj["debug"]) {
        return R"([{
                    "username": "user1",
                    "group_id": "group_id1"
                    }])"_json;
    } else {
        return retj["list"];
    }
}

json ClientUtils::send_add_friend_result(const std::string &from, const std::string &to, bool accept) {
    Thread::stop_recv();

    json j;
    j["command"] = "send_add_friend_result";
    j["from"] = from;
    j["to"] = to;
    j["accept"] = accept;

    Client::getIns()->send_string(j.dump());

    memset(buf, 0, sizeof(buf));
    Client::getIns()->recv_blocked(buf);

    Thread::resume_recv();

    printf("server: %s\n", buf);

    return json::parse(buf);
}

json ClientUtils::send_invite_to_group_result(const std::string &whom, const std::string &group_id, bool accept) {
    Thread::stop_recv();

    json j;
    j["command"] = "send_invite_to_group_result";
    j["whom"] = whom;
    j["group_id"] = group_id;
    j["accept"] = accept;

    Client::getIns()->send_string(j.dump());

    Client::getIns()->recv_blocked(buf);

    Thread::resume_recv();

    return json::parse(buf);
}

json ClientUtils::send_join_group_result(const std::string &who, const std::string &group_id, bool accept) {
    Thread::stop_recv();

    json j;
    j["command"] = "send_join_group_result";
    j["who"] = who;
    j["group_id"] = group_id;
    j["accept"] = accept;

    Client::getIns()->send_string(j.dump());

    Client::getIns()->recv_blocked(buf);

    Thread::resume_recv();

    return json::parse(buf);
}

// 阻塞的 send chat msg
json ClientUtils::chat_send_msg(const std::string & from, const std::string & to, const std::string & msg) {
    // 阻塞接收
    Thread::stop_recv();

    json j;
    j["command"] = "chat_send_msg";
    j["from"] = from;
    j["to"] = to;
    j["content"] = msg;

    Client::getIns()->send_string(j.dump());

//    char buf[CLIENT_BUF_SIZE];
    Client::getIns()->recv_blocked(buf);


    Thread::resume_recv();


    return json::parse(buf);
}

// 阻塞的 group send msg
json ClientUtils::group_send_msg(const std::string & from, const std::string & group_id, const std::string & msg) {
    // 阻塞接收
    Thread::stop_recv();

    json j;
    j["command"] = "group_send_msg";
    j["from"] = from;
    j["group_id"] = group_id;
    j["content"] = msg;

    Client::getIns()->send_string(j.dump());

//    char buf[CLIENT_BUF_SIZE];
    Client::getIns()->recv_blocked(buf);


    Thread::resume_recv();


    return json::parse(buf);
}

json ClientUtils::create_group(const std::string &who, const std::string &group_name) {
    Thread::stop_recv();

    json j;
    j["command"] = "create_group";
    j["who"] = who;
    j["group_name"] = group_name;

    Client::getIns()->send_string(j.dump());

//    char buf[CLIENT_BUF_SIZE];
    Client::getIns()->recv_blocked(buf);


    Thread::resume_recv();


    return json::parse(buf);
}

json ClientUtils::ls_unread_msg(const std::string &who) {
    Thread::stop_recv();

    json j;
    j["command"] = "ls_unread_msg";
    j["username"] = who;
//    j["group_name"] = group_name;

    Client::getIns()->send_string(j.dump());

//    char buf[CLIENT_BUF_SIZE];
    Client::getIns()->recv_blocked(buf);


    Thread::resume_recv();


    return json::parse(buf);
}