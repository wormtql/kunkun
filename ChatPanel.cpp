//
// Created by iatow on 19-8-28.
//

#include "INCLUDE.h"

#include "ChatPanel.h"

ChatPanel::ChatPanel() {
    this->root = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(root, "chat_panel");

    this->side_bar = create_side_bar();

    gtk_box_pack_start(GTK_BOX(root), side_bar, FALSE, FALSE, 0);

    this->body = create_body();

    gtk_box_pack_start(GTK_BOX(root), body, TRUE, TRUE, 0);
}

GtkWidget* ChatPanel::widget() {
    return root;
}

ChatPanel* ChatPanel::create() {
    return new ChatPanel();
}

GtkWidget* ChatPanel::create_side_bar() {
//    side_bar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
//    side_bar = gtk_list_box_new();
    side_bar = gtk_scrolled_window_new(nullptr, nullptr);
    GtkWidget * box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    gtk_widget_set_name(side_bar, "side_bar");
    gtk_container_add(GTK_CONTAINER(side_bar), box);

    GtkWidget * label = gtk_label_new("my friends");
    gtk_widget_set_name(label, "chat_panel_side_header");
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

//    gtk_list_box_prepend(GTK_LIST_BOX(side_bar), label);

    gtk_box_pack_start(GTK_BOX(box), create_side_item(nullptr, "worm"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), create_side_item(nullptr, "worm"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), create_side_item(nullptr, "worm"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), create_side_item(nullptr, "worm"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), create_side_item(nullptr, "worm"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), create_side_item(nullptr, "worm"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), create_side_item(nullptr, "worm"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), create_side_item(nullptr, "worm"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), create_side_item(nullptr, "worm"), FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), create_side_item(nullptr, "worm"), FALSE, FALSE, 0);



    return side_bar;
}

GtkWidget* ChatPanel::create_side_item(GdkPixbuf *pixbuf, const char *name) {
    GtkWidget * image;

    if (pixbuf) {
        image = gtk_image_new_from_pixbuf(pixbuf);
    } else {
//        image = gtk_image_new_from_icon_name("go-up", GTK_ICON_SIZE_BUTTON);

        auto pix = gdk_pixbuf_new_from_file_at_size("../assets/test.jpg", 48, 48, nullptr);
        
        image = gtk_image_new_from_pixbuf(pix);
    }
    GtkWidget * label = gtk_label_new(name);

    GtkWidget * button = gtk_button_new();

    GtkWidget * box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_box_pack_start(GTK_BOX(box), image, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

    gtk_container_add(GTK_CONTAINER(button), box);

    return button;
}

GtkWidget* ChatPanel::create_body() {
    body = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(body, "chat_panel_body");

    chat_body = gtk_scrolled_window_new(nullptr, nullptr);
    gtk_widget_set_name(chat_body, "chat_panel_chat_body");

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

GtkWidget* ChatPanel::create_button_set_1() {
    GtkWidget * box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(box, "chat_panel_button_set_1");

    GtkWidget * button_file = gtk_button_new_from_icon_name("go_left", GTK_ICON_SIZE_BUTTON);
    gtk_widget_set_name(button_file, "chat_panel_button_file");
    g_signal_connect(button_file, "clicked", G_CALLBACK(ChatPanel::on_button_file_clicked), this);

    GtkWidget * button_image = gtk_button_new_from_icon_name("go_left", GTK_ICON_SIZE_BUTTON);
    gtk_widget_set_name(button_image, "chat_panel_button_image");
    g_signal_connect(button_image, "clicked", G_CALLBACK(ChatPanel::on_button_image_clicked), this);

    gtk_box_pack_start(GTK_BOX(box), button_file, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), button_image, FALSE, FALSE, 0);

    return box;
}

GtkWidget* ChatPanel::create_button_set_2() {
    GtkWidget * box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(box, "chat_panel_button_set_2");

    GtkWidget * button_send = gtk_button_new_with_label("send");
    gtk_widget_set_name(button_send, "chat_panel_button_send");
    g_signal_connect(button_send, "clicked", G_CALLBACK(ChatPanel::on_button_send_clicked), this);

    gtk_box_pack_end(GTK_BOX(box), button_send, FALSE, FALSE, 0);

    return box;
}

void ChatPanel::on_button_file_clicked(GtkWidget *widget, gpointer data) {
    printf("aaa\n");
}

void ChatPanel::on_button_image_clicked(GtkWidget *widget, gpointer data) {

}

void ChatPanel::on_button_send_clicked(GtkWidget *widget, gpointer data) {

}