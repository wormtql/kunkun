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

    // box
    GtkWidget * root = nullptr;

    // scrolled window
    GtkWidget * side_bar = nullptr;

    // box
    GtkWidget * body = nullptr;

    // 消息框
    GtkWidget * chat_body = nullptr;

    // input box
    GtkWidget * input_box = nullptr;
    GtkTextBuffer * input_box_buffer = nullptr;


    ChatPanel();

    GtkWidget * create_side_bar();

    GtkWidget * create_side_item(GdkPixbuf * pixbuf, const char * name);

    GtkWidget * create_body();

    GtkWidget * create_button_set_1();

    GtkWidget * create_button_set_2();

    void create_all_side_items();

    static void on_button_file_clicked(GtkWidget * widget, gpointer data);

    static void on_button_image_clicked(GtkWidget * widget, gpointer data);

    static void on_button_send_clicked(GtkWidget * widget, gpointer data);

public:
    static ChatPanel * create();

    GtkWidget * widget();
};


#endif //KUNKUN_CHATPANEL_H
