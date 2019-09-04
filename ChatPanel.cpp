//
// Created by iatow on 19-8-28.
//

#include "INCLUDE.h"

#include "ChatPanel.h"


using std::vector;
using std::string;


ChatPanel::ChatPanel() {
    this->root = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(root, "chat_panel");

    GtkWidget * temp_scroll = create_side_bar();

    gtk_box_pack_start(GTK_BOX(root), temp_scroll, FALSE, FALSE, 0);

    this->body = create_body();

    gtk_box_pack_start(GTK_BOX(root), body, TRUE, TRUE, 0);


//    InvokeData * data = new InvokeData;
//    data->chat_panel = this;
//    data->context = g_main_context_get_thread_default();

    GMainContext * context = g_main_context_get_thread_default();

    Thread::set_call_back("chat_recv_msg", [this, context] (char * da, int size) {

        std::string data = da;

        json j = json::parse(data);
        auto d = new InvokeData;
        d->chat_panel = this;
        d->context = context;
        if (j["debug"]) {
            d->item = R"({"command": "chat_recv_msg",
                          "from": "user_remote",
                          "to": "",
                          "content": {"type": "text", "content": "text test"}})"_json;
        } else {
            d->item = j;
        }
//
//        json item = d->item;
//
//        if (this->current_type == "friend" && this->current_name == item["from"]) {
//            this->append_chat_item(nullptr, item, item["from"] == DataHub::getIns()->username);
//        } else {
//            this->mark_new_msg("friend", item["from"]);
//        }

        InvokeData * idata = new InvokeData;
        idata->context = context;
        idata->chat_panel = this;
        idata->item = j;

        g_main_context_invoke(context, [] (gpointer user_data) -> gboolean {

            printf("invoked\n");

            auto data = (InvokeData *)user_data;
            auto panel = data->chat_panel;
            auto item = data->item;


            if (panel->current_type == "friend" && panel->current_name == item["from"]) {

                json content = item["content"];
                if (content["type"] == "unread") {
                    printf("unread\n");
                    panel->mark_new_msg("friend", item["from"]);
                } else {
                    panel->append_chat_item(nullptr, content, item["from"] == DataHub::getIns()->username);
                    panel->refresh_chat_body();
                    printf("current\n");
                }
            } else {
                printf("not current\n");
                panel->mark_new_msg("friend", item["from"]);
            }


            return false;
        }, idata);
    });

    Thread::set_call_back("group_recv_msg", [this, context] (char * da, int size) {

        std::string data = da;

        json j = json::parse(data);
        auto d = new InvokeData;
        d->chat_panel = this;
        d->context = context;
        if (j["debug"]) {
            d->item = R"({"command": "chat_recv_msg",
                          "from": "user_remote",
                          "group_id": "group_id1",
                          "content": {"type": "text", "content": "text test"}})"_json;
        } else {
            d->item = j;
        }

//        json item = d->item;
//
//        if (this->current_type == "group" && this->current_name == item["group_id"]) {
//            this->append_chat_item(nullptr, item["content"], item["from"] == DataHub::getIns()->username);
//        } else {
//            this->mark_new_msg("group", item["content"]["group_id"]);
//        }

        g_main_context_invoke(context, [] (gpointer user_data) -> gboolean {

            auto data = (InvokeData *)user_data;
            auto panel = data->chat_panel;
            auto item = data->item;


            if (panel->current_type == "group" && panel->current_name == item["group_id"]) {

                json content = item["content"];
                if (content["type"] == "unread") {
                    panel->mark_new_msg("group", item["group_id"]);
                    printf("unread\n");
                } else {
                    printf("current\n");
                    panel->append_chat_item(nullptr, item["content"], item["from"] == DataHub::getIns()->username);
                    panel->refresh_chat_body();
                }
            } else {
                printf("not current\n");
                panel->mark_new_msg("group", item["group_id"]);
            }


            return false;
        }, d);

    });

}

GtkWidget* ChatPanel::widget() {
    return root;
}

ChatPanel* ChatPanel::create() {
    return new ChatPanel();
}


/*
 * function: create friends list
 *
 */
GtkWidget* ChatPanel::create_side_bar() {
//    side_bar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
//    side_bar = gtk_list_box_new();
    GtkWidget * temp_scroll = gtk_scrolled_window_new(nullptr, nullptr);
//    side_bar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    side_bar = gtk_list_box_new();
    gtk_list_box_set_selection_mode(GTK_LIST_BOX(side_bar), GTK_SELECTION_NONE);

    gtk_widget_set_name(temp_scroll, "side_bar");
    gtk_container_add(GTK_CONTAINER(temp_scroll), side_bar);


    return temp_scroll;
}


/*
 * function: create single friend item widget
 *
 */
GtkWidget* ChatPanel::create_side_item(GdkPixbuf *pixbuf, const char *name) {
    GtkWidget * image;

    if (pixbuf) {
        image = gtk_image_new_from_pixbuf(pixbuf);
    } else {
        auto pix = gdk_pixbuf_new_from_file_at_size("../assets/test.jpg", 48, 48, nullptr);
        pix = Utils::get_clipped_image(pix, 48);
        
        image = gtk_image_new_from_pixbuf(pix);
    }
    GtkWidget * label = gtk_label_new(name);

    GtkWidget * button = gtk_button_new();
    Utils::add_css_class(button, "worm");

    GtkWidget * box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(box), image, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

    gtk_container_add(GTK_CONTAINER(button), box);


//    g_signal_connect(button, "clicked", G_CALLBACK(ChatPanel::on_button_side_item_clicked), this);

    return button;
}


/*
 * function: create chat body
 *
 */
GtkWidget* ChatPanel::create_body() {
    // body
    body = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(body, "chat_panel_body");

    chat_body = gtk_scrolled_window_new(nullptr, nullptr);
    gtk_widget_set_name(chat_body, "chat_panel_chat_body");
    chat_body_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(chat_body_box, "chat_panel_chat_body_box");
    gtk_container_add(GTK_CONTAINER(chat_body), chat_body_box);

    gtk_box_pack_start(GTK_BOX(body), chat_body, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(body), create_button_set_1(), FALSE, FALSE, 0);

    GtkWidget * temp_scroll = gtk_scrolled_window_new(nullptr, nullptr);
    input_box_buffer = gtk_text_buffer_new(nullptr);
    input_box = gtk_text_view_new_with_buffer(input_box_buffer);
    gtk_container_add(GTK_CONTAINER(temp_scroll), input_box);
    gtk_widget_set_name(input_box, "chat_panel_input_box");

    gtk_box_pack_start(GTK_BOX(body), temp_scroll, TRUE, TRUE, 0);

    gtk_box_pack_end(GTK_BOX(body), create_button_set_2(), FALSE, FALSE, 0);


    return body;
}


/*
 * create button set 1
 *
 */
GtkWidget* ChatPanel::create_button_set_1() {
    GtkWidget * box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(box, "chat_panel_button_set_1");

//    GtkWidget * button_file = gtk_button_new_from_icon_name("go_left", GTK_ICON_SIZE_BUTTON);
    GtkWidget * button_file = Utils::create_button_with_icon("../assets/plane_brown.png", 24);
    gtk_widget_set_name(button_file, "chat_panel_button_file");

    g_signal_connect(button_file, "clicked", G_CALLBACK(ChatPanel::on_button_file_clicked), this);

//    GtkWidget * button_image = gtk_button_new_from_icon_name("go_left", GTK_ICON_SIZE_BUTTON);
    GtkWidget * button_image = Utils::create_button_with_icon("../assets/image_brown.png", 24);
    gtk_widget_set_name(button_image, "chat_panel_button_image");
    g_signal_connect(button_image, "clicked", G_CALLBACK(ChatPanel::on_button_image_clicked), this);

    gtk_box_pack_start(GTK_BOX(box), button_file, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), button_image, FALSE, FALSE, 0);

    return box;
}


/*
 * create button set 2
 *
 */
GtkWidget* ChatPanel::create_button_set_2() {
    button_set_2 = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(button_set_2, "chat_panel_button_set_2");

    GtkWidget * button_send = gtk_button_new_with_label("send");
    gtk_widget_set_name(button_send, "chat_panel_button_send");
    g_signal_connect(button_send, "clicked", G_CALLBACK(ChatPanel::on_button_send_clicked), this);

    gtk_box_pack_end(GTK_BOX(button_set_2), button_send, FALSE, FALSE, 0);


//    progress_bar = gtk_progress_bar_new();
//    gtk_widget_set_name(progress_bar, "progress_bar");
//    gtk_box_pack_start(GTK_BOX(button_set_2), progress_bar, TRUE, TRUE, 0);
//    gtk_widget_show_all(panel->progress_bar);

    return button_set_2;
}


/*
 * send file
 *
 */
void ChatPanel::on_button_file_clicked(GtkWidget *widget, gpointer data) {
//    printf("aaa\n");

    auto panel = (ChatPanel *)data;

    GtkWidget * dialog;
    GtkFileChooserAction action = GTK_FILE_CHOOSER_ACTION_OPEN;

    dialog = gtk_file_chooser_dialog_new(
            "Open File",
            GTK_WINDOW(panel->parent_window),
            action,
            "Cancel",
            GTK_RESPONSE_CANCEL,
            "Open",
            GTK_RESPONSE_ACCEPT,
            NULL);

    int res = gtk_dialog_run(GTK_DIALOG(dialog));
    if (res == GTK_RESPONSE_ACCEPT)
    {
        std::string filename;
        GtkFileChooser * chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);

        printf("%s\n", filename.c_str());


        std::string fileid;
        bool status = false;
        ClientUtils::send_file_begin(Utils::get_filename_from_path(filename),
                                     [&fileid, &status] (char * da, int size) {
                                         std::string ret = da;

                                         json j = json::parse(ret);

                                         if (j["debug"]) {
                                             fileid = "debug";
                                             status = true;
                                         } else {
                                             fileid = j["fileid"];
                                             status = j["status"];
                                         }

                                     });


        GMainContext * context = g_main_context_get_thread_default();

        if (panel->current_type == "friend") {
            if (status) {
                SendFileUtils::chat_send_file(DataHub::getIns()->username,
                        panel->current_name,
                        filename, fileid, [&panel, filename, fileid, context] (double rate) {
                    printf("%lf\n", rate);

                    if (rate < 1.0) {
                        return;
                    }

                    ChatData * data = new ChatData;
                    data->chat_panel = panel;
                    data->file_id = fileid;
                    data->file_name = filename;

                    g_main_context_invoke(context, [] (gpointer user_data) -> gboolean {


                        auto d = (ChatData *)user_data;
                        auto panel = d->chat_panel;

                        GtkWidget * lbl = gtk_label_new("文件发送完成");
                        gtk_widget_set_name(lbl, "status_bar");
                        gtk_box_pack_start(GTK_BOX(panel->button_set_2), lbl, TRUE, TRUE, 0);
                        gtk_widget_show_all(lbl);

                        json j;
                        j["type"] = "file";
                        j["file_name"] = Utils::get_filename_from_path(d->file_name);
                        j["file_id"] = d->file_id;
                        panel->append_chat_item(nullptr, j, true);
                        panel->refresh_chat_body();

                        g_timeout_add(5000, [] (gpointer user_data) -> gboolean {

                            auto p = (GtkWidget *)user_data;
                            if (GTK_IS_WIDGET(p)) {
                                gtk_widget_destroy(p);
                            }


                            return false;

                        }, lbl);

                        return false;
                    }, data);


                });
            } else {
                printf("error in send file\n");
            }
        } else if (panel->current_type == "group") {

            if (status) {
                SendFileUtils::group_send_file(DataHub::getIns()->username, panel->current_name, filename, fileid,
                        [panel, filename, fileid, context] (double rate) {
//                    printf("%lf\n", rate);

                    if (rate < 1.0) {
                        return;
                    }

//                    if (rate >= 1.0) {

                    ChatData * da = new ChatData;
                    da->file_id = fileid;
                    da->file_name = filename;
                    da->chat_panel = panel;
                    g_main_context_invoke(context, [] (gpointer user_data) -> gboolean {

                        auto data = (ChatData *)user_data;
                        auto panel = data->chat_panel;

                        GtkWidget * lbl = gtk_label_new("文件发送完成");
                        gtk_widget_set_name(lbl, "status_bar");
                        gtk_box_pack_start(GTK_BOX(panel->button_set_2), lbl, TRUE, TRUE, 0);
                        gtk_widget_show_all(lbl);

                        json j;
                        j["type"] = "file";
                        j["file_name"] = Utils::get_filename_from_path(data->file_name);
                        j["file_id"] = data->file_id;
                        panel->append_chat_item(nullptr, j, true);
                        panel->refresh_chat_body();

                        g_timeout_add(5000, [] (gpointer user_data) -> gboolean {

                            auto p = (GtkWidget *)user_data;
                            if (GTK_IS_WIDGET(p)) {
                                gtk_widget_destroy(p);
                            }


                            return false;

                        }, lbl);
                    }, da);
//                    }
                });
            } else {
                printf("error in send file\n");
            }
        }

//        g_free(filename);
    }

    gtk_widget_destroy(dialog);
}

void ChatPanel::on_button_image_clicked(GtkWidget *widget, gpointer data) {

}

/*
 * 发送
 *
 */
void ChatPanel::on_button_send_clicked(GtkWidget *widget, gpointer data) {
    auto panel = (ChatPanel *)data;

    // to whom, group or friend
    string type = panel->current_type;
    string id = panel->current_name;

    GtkTextIter start, end;
    gtk_text_buffer_get_iter_at_offset(panel->input_box_buffer, &start, 0);
    gtk_text_buffer_get_iter_at_offset(panel->input_box_buffer, &end, 1000000);

    // text to be sent
    std::string text = gtk_text_buffer_get_text(panel->input_box_buffer, &start, &end, TRUE);

    printf("%s\n", text.c_str());

    // append chat item
    json temp;
    temp["type"] = "text";
    temp["content"] = text;
    panel->append_chat_item(Utils::load_from_file("../assets/test.jpg", 48), temp, true);

    // clear send input box
    gtk_text_buffer_set_text(panel->input_box_buffer, "", 0);


    // send
    if (type == "friend")
    {
//        ClientUtils::send_text_to_friend(DataHub::getIns()->username, id, text);
        ClientUtils::chat_send_msg(DataHub::getIns()->username, id, text);
    }
    else if (type == "group")
    {
//        ClientUtils::send_text_to_group(DataHub::getIns()->username, id, text);
        ClientUtils::group_send_msg(DataHub::getIns()->username, id, text);
    }
    panel->refresh_chat_body();
}


/*
 * choose a friend or a group
 *
 */
void ChatPanel::on_button_side_item_clicked(GtkWidget *widget, gpointer d) {
    auto data = (Data *)d;
    auto panel = data->chat_panel;

//    Utils::remove_css_class(widget, "new_msg");

    GtkWidget * box = gtk_bin_get_child(GTK_BIN(widget));
    GList * list = gtk_container_get_children(GTK_CONTAINER(box));
    int iter = 0;
    while (list) {
        if (GTK_IS_IMAGE(list->data) && iter == 2) {
            gtk_widget_destroy(GTK_WIDGET(list->data));
        }
        iter++;
        list = list->next;
    }


    if (panel->current_friend) {
        Utils::remove_css_class(panel->current_friend, "chosen_friend");
        Utils::add_css_class(panel->current_friend, "worm");
    }
    panel->current_friend = widget;
    Utils::remove_css_class(panel->current_friend, "worm");
    Utils::add_css_class(panel->current_friend, "chosen_friend");


    panel->current_type = data->type;
    std::cout << data->type << std::endl;
    panel->current_name = data->id;
    std::cout << data->id << std::endl;


    panel->clear_chat_item();

    json j;
    if (data->type == "friend")
    {
        j = ClientUtils::get_chat_friend_history(DataHub::getIns()->username, data->id);
    }
    else if (data->type == "group")
    {
        j = ClientUtils::get_chat_group_history(DataHub::getIns()->username, data->id);
    }

    for (json item : j)
    {
        bool self = item["name"] == DataHub::getIns()->username;
        panel->append_chat_item(nullptr, item, self);
    }

    panel->refresh_chat_body();

//    gtk_box_s
}


/*
 * append chat message
 * a text or a file button
 */
void ChatPanel::append_chat_item(GdkPixbuf * pix, const json & j, bool self) {

    if (pix == nullptr)
    {
        pix = gdk_pixbuf_new_from_file_at_size("../assets/test.jpg", 48, 48, nullptr);
        pix = Utils::get_clipped_image(pix, 48);
    }

    GtkWidget * item = create_chat_item(pix, j, self);
    Utils::add_css_class(item, "chat_panel_chat_item");

    gtk_box_pack_start(GTK_BOX(this->chat_body_box), item, FALSE, FALSE, 0);

//    gtk_widget_show_all(item);
}


/*
 * create a chat item
 * a text or a file button
 */
GtkWidget* ChatPanel::create_chat_item(GdkPixbuf * pix, const json & j, bool self) {
    GtkWidget * box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

//    std::cout << j << std::endl;

//    json item = j["content"];

    if (j["type"] == "text")
    {
        GtkWidget * image = gtk_image_new_from_pixbuf(pix);
        GtkWidget * label = gtk_label_new(((string)j["content"]).c_str());
        if (self)
        {
            Utils::add_css_class(image, "chat_avatar_right");
            Utils::add_css_class(label, "chat_content_right");
            gtk_box_pack_end(GTK_BOX(box), image, FALSE, FALSE, 0);
            gtk_box_pack_end(GTK_BOX(box), label, FALSE, FALSE, 0);

            return box;
        }
        else
        {
            Utils::add_css_class(image, "chat_avatar_left");
            Utils::add_css_class(label, "chat_content_left");
            gtk_box_pack_start(GTK_BOX(box), image, FALSE, FALSE, 0);
            gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

            return box;
        }
    }
    else if (j["type"] == "file")
    {
        GtkWidget * image = gtk_image_new_from_pixbuf(pix);
        GtkWidget * btn = gtk_button_new_with_label(("文件：" + (string)j["file_name"]).c_str());

        ChatData * data = new ChatData;
        data->chat_panel = this;
        data->file_id = j["file_id"];
        data->file_name = j["file_name"];
        g_signal_connect(btn, "clicked", G_CALLBACK(ChatPanel::on_button_recv_file_clicked), data);
        if (self)
        {
            Utils::add_css_class(image, "chat_avatar_right");
            Utils::add_css_class(btn, "chat_button_file_right");
            gtk_box_pack_end(GTK_BOX(box), image, FALSE, FALSE, 0);
            gtk_box_pack_end(GTK_BOX(box), btn, FALSE, FALSE, 0);

            return box;
        }
        else
        {
            Utils::add_css_class(image, "chat_avatar_left");
            Utils::add_css_class(btn, "chat_button_file_left");
            gtk_box_pack_start(GTK_BOX(box), image, FALSE, FALSE, 0);
            gtk_box_pack_start(GTK_BOX(box), btn, FALSE, FALSE, 0);

            return box;
        }
    }
}

void ChatPanel::on_button_recv_file_clicked(GtkWidget *widget, gpointer user_data) {
    auto data = (ChatData *)user_data;

    auto file_id = data->file_id;

    ClientUtils::send_me_a_file(file_id);

//    printf("%s\n", data->file_id.c_str());
}

void ChatPanel::clear_chat_item() {
    GList * list = gtk_container_get_children(GTK_CONTAINER(this->chat_body_box));
    while (list)
    {
        gtk_container_remove(GTK_CONTAINER(this->chat_body_box), (GtkWidget *)list->data);
        list = list->next;
    }


}

void ChatPanel::refresh_friends_list(json friends, json groups) {
    // clear previous items
    GList * list = gtk_container_get_children(GTK_CONTAINER(this->side_bar));
    while (list)
    {
        gtk_container_remove(GTK_CONTAINER(this->side_bar), (GtkWidget *)list->data);
        list = list->next;
    }
    username_to_widget.clear();
    group_id_to_widget.clear();


    // retrieve different tags
    vector<string> my_friend;
    vector<string> blacklist;
    vector<string> special;
    for (json & j : friends)
    {
        if (j["tag"] == "my_friend")
        {
            my_friend.push_back(j["name"]);
        }
        else if (j["tag"] == "blacklist")
        {
            blacklist.push_back(j["name"]);
        }
        else if (j["tag"] == "special")
        {
            special.push_back(j["name"]);
        }
    }


    GtkWidget * label_my_friend = gtk_label_new("我的好友");

    GtkWidget * label_blacklist = gtk_label_new("黑名单");

    GtkWidget * label_special = gtk_label_new("特别关心");

    GtkWidget * label_group = gtk_label_new("群聊");

    Utils::add_css_class(label_my_friend, "chat_panel_side_header");
    Utils::add_css_class(label_blacklist, "chat_panel_side_header");
    Utils::add_css_class(label_special, "chat_panel_side_header");
    Utils::add_css_class(label_group, "chat_panel_side_header");

    for (string & s: blacklist)
    {
        GtkWidget * item = create_side_item(nullptr, s.c_str());
        username_to_widget[s] = item;
        Data * data = new Data;
        data->chat_panel = this;
        data->name = s;
        data->type = "friend";
        data->id = s;
        g_signal_connect(item, "clicked", G_CALLBACK(ChatPanel::on_button_side_item_clicked), data);
        gtk_list_box_prepend(GTK_LIST_BOX(side_bar), item);
    }
    gtk_list_box_prepend(GTK_LIST_BOX(side_bar), label_blacklist);


    for (json & j: groups)
    {
        GtkWidget * item = create_side_item(nullptr, ((string)j["name"]).c_str());
        group_id_to_widget[(string)j["id"]] = item;
        Data * data = new Data;
        data->chat_panel = this;
        data->name = j["name"];
        data->type = "group";
        data->id = j["id"];
        g_signal_connect(item, "clicked", G_CALLBACK(ChatPanel::on_button_side_item_clicked), data);
        gtk_list_box_prepend(GTK_LIST_BOX(side_bar), item);
    }
    gtk_list_box_prepend(GTK_LIST_BOX(side_bar), label_group);


    for (string & s: my_friend)
    {
        GtkWidget * item = create_side_item(nullptr, s.c_str());
        username_to_widget[s] = item;
        Data * data = new Data;
        data->chat_panel = this;
        data->name = s;
        data->type = "friend";
        data->id = s;
        g_signal_connect(item, "clicked", G_CALLBACK(ChatPanel::on_button_side_item_clicked), data);
        gtk_list_box_prepend(GTK_LIST_BOX(side_bar), item);
    }
    gtk_list_box_prepend(GTK_LIST_BOX(side_bar), label_my_friend);


    for (string & s: special)
    {
        GtkWidget * item = create_side_item(nullptr, s.c_str());
        username_to_widget[s] = item;
        Data * data = new Data;
        data->chat_panel = this;
        data->name = s;
        data->type = "friend";
        data->id = s;
        g_signal_connect(item, "clicked", G_CALLBACK(ChatPanel::on_button_side_item_clicked), data);
        gtk_list_box_prepend(GTK_LIST_BOX(side_bar), item);
    }
    gtk_list_box_prepend(GTK_LIST_BOX(side_bar), label_special);


    gtk_widget_show_all(side_bar);
}

void ChatPanel::refresh_chat_body() {
    gtk_widget_show_all(this->chat_body_box);
}

void ChatPanel::set_parent_window(GtkWidget *window) {
    this->parent_window = window;
}

void ChatPanel::mark_new_msg(const std::string &type, const std::string &id) {
    printf("new_msg");

    printf("id: %s\n", id.c_str());

    if (type == "friend") {
        GtkWidget * widget = username_to_widget[id];

        GtkWidget * image = gtk_image_new_from_pixbuf(Utils::create_round_dot_pix(14, 220, 20, 60));
        Utils::add_css_class(image, "new_msg");

        GtkWidget * box = gtk_bin_get_child(GTK_BIN(widget));
        gtk_box_pack_end(GTK_BOX(box), image, FALSE, FALSE, 0);

        gtk_widget_show_all(widget);
    } else if (type == "group") {
        GtkWidget * widget = group_id_to_widget[id];

        GtkWidget * image = gtk_image_new_from_pixbuf(Utils::create_round_dot_pix(14, 220, 20, 60));
        Utils::add_css_class(image, "new_msg");

        GtkWidget * box = gtk_bin_get_child(GTK_BIN(widget));
        gtk_box_pack_end(GTK_BOX(box), image, FALSE, FALSE, 0);

        gtk_widget_show_all(widget);
    }
}

void ChatPanel::mark_all_new_msg() {
    json unread = ClientUtils::ls_unread_msg(DataHub::getIns()->username);
    json list = unread["list"];

    for (json j : list) {
        if (j["type"] == "friend") {
            this->mark_new_msg("friend", j["who"]);
        } else if (j["type"] == "group") {
            this->mark_new_msg("group", j["group_id"]);
        }
    }
}