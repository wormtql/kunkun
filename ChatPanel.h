//
// Created by iatow on 19-8-28.
//

#ifndef KUNKUN_CHATPANEL_H
#define KUNKUN_CHATPANEL_H

#ifndef JSON
#define JSON
using json = nlohmann::json;
#endif

class ChatPanel {
private:
    // side item data
    struct Data
    {
        ChatPanel * chat_panel;
        std::string type;
        std::string name;
        std::string id;
    };
    // chat item data
    struct ChatData {
        ChatPanel * chat_panel;
        std::string file_name;
        std::string file_id;
    };

    // invoke struct
    struct InvokeData {
        ChatPanel * chat_panel;
        GMainContext * context;
        json item;
    };
private:

    GtkWidget * parent_window = nullptr;

    // box
    GtkWidget * root = nullptr;

    // box, friends list
    GtkWidget * side_bar = nullptr;
    std::map<std::string, GtkWidget *> username_to_widget;
    std::map<std::string, GtkWidget *> group_id_to_widget;

    // box
    GtkWidget * body = nullptr;

    // 消息框
    GtkWidget * chat_body = nullptr;
    GtkWidget * chat_body_box = nullptr;

    // input box
    GtkWidget * input_box = nullptr;
    GtkTextBuffer * input_box_buffer = nullptr;

    GtkWidget * status_bar = nullptr;

    GtkWidget * button_set_2 = nullptr;


    GtkWidget * current_friend = nullptr;

    std::string current_type = "";
    std::string current_name = "";


    ChatPanel();

    GtkWidget * create_side_bar();

    GtkWidget * create_side_item(GdkPixbuf * pixbuf, const char * name);

    GtkWidget * create_body();

    GtkWidget * create_button_set_1();

    GtkWidget * create_button_set_2();


    GtkWidget * create_chat_item(GdkPixbuf * pix, const json & item, bool self);

    static void on_button_file_clicked(GtkWidget * widget, gpointer data);

    static void on_button_image_clicked(GtkWidget * widget, gpointer data);

    static void on_button_send_clicked(GtkWidget * widget, gpointer data);

    static void on_button_side_item_clicked(GtkWidget * widget, gpointer data);


    static void on_button_recv_file_clicked(GtkWidget * widget, gpointer data);

public:
    static ChatPanel * create();

    GtkWidget * widget();


    void append_chat_item(GdkPixbuf * pix, const json & item, bool self);
    void clear_chat_item();


    void refresh_friends_list(json friends, json groups);


    void refresh_chat_body();


    void mark_new_msg(str type, str id);


    void set_parent_window(GtkWidget * window);


    void mark_all_new_msg();
};


#endif //KUNKUN_CHATPANEL_H
