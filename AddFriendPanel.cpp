//
// Created by iatow on 19-8-29.
//
#include "INCLUDE.h"
#include "AddFriendPanel.h"


AddFriendPanel::AddFriendPanel() {
    root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(root, "add_friend_panel");


    GtkWidget * label = gtk_label_new("发现身边的IKUN");
    gtk_widget_set_name(label, "add_friend_panel_header");
    gtk_box_pack_start(GTK_BOX(root), label, FALSE, FALSE, 0);


    search_bar = gtk_search_entry_new();
    gtk_widget_set_name(search_bar, "search_bar");
    gtk_entry_set_placeholder_text(GTK_ENTRY(search_bar), "发现那个IKUN");

    gtk_box_pack_start(GTK_BOX(root), search_bar, FALSE, FALSE, 0);


    GtkWidget * temp_scroll = gtk_scrolled_window_new(nullptr, nullptr);
    gtk_widget_set_name(temp_scroll, "search_result_scroll");

    search_result = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(search_result, "search_result");
    gtk_container_add(GTK_CONTAINER(temp_scroll), search_result);

    gtk_box_pack_start(GTK_BOX(root), temp_scroll, TRUE, TRUE, 0);



}

GtkWidget* AddFriendPanel::widget() {
    return root;
}

AddFriendPanel* AddFriendPanel::create() {
    return new AddFriendPanel;
}