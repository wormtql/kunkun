//
// Created by iatow on 19-8-29.
//
#include "INCLUDE.h"
#include "AddFriendPanel.h"


AddFriendPanel::AddFriendPanel() {
    root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(root, "add_friend_panel");


//    GtkWidget * label = gtk_label_new("发现身边的IKUN");
//    gtk_widget_set_name(label, "add_friend_panel_header");
//    gtk_box_pack_start(GTK_BOX(root), label, FALSE, FALSE, 0);

//    GtkWidget * image = Utils::create_image_from_file_at_size("../assets/kunkun.png", -1);
    GtkWidget * image = Utils::create_image_from_file_at_size("../assets/kunkun.png", 240, 180);
    gtk_box_pack_start(GTK_BOX(root), image, FALSE, FALSE, 0);


    search_bar = gtk_search_entry_new();
    search_buffer = gtk_entry_get_buffer(GTK_ENTRY(search_bar));
    gtk_widget_set_name(search_bar, "search_bar");
    gtk_entry_set_placeholder_text(GTK_ENTRY(search_bar), "发现那个IKUN");
    g_signal_connect(search_bar, "search-changed", G_CALLBACK(AddFriendPanel::on_text_changed), this);

    gtk_box_pack_start(GTK_BOX(root), search_bar, FALSE, FALSE, 0);


    GtkWidget * temp_scroll = gtk_scrolled_window_new(nullptr, nullptr);
    gtk_widget_set_name(temp_scroll, "search_result_scroll");

    search_result = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(search_result, "search_result");
    gtk_container_add(GTK_CONTAINER(temp_scroll), search_result);

    gtk_box_pack_start(GTK_BOX(root), temp_scroll, FALSE, FALSE, 0);


}

GtkWidget* AddFriendPanel::widget() {
    return root;
}

AddFriendPanel* AddFriendPanel::create() {
    return new AddFriendPanel;
}

void AddFriendPanel::on_text_changed(GtkSearchEntry *entry, gpointer data) {

    auto panel = (AddFriendPanel *)data;

    panel->clear_search_result();

    std::string text = gtk_entry_get_text(GTK_ENTRY(entry));

    if (text.empty())
    {
        return;
    }


    json j = ClientUtils::get_list_user_like_blocked(text);

    for (json & item : j)
    {
        std::string res = item.get<std::string>();

        if (res == DataHub::getIns()->username) {
            continue;
        }

        GtkWidget * widget = panel->create_search_result_item_friend(res, nullptr);
        panel->add_search_result_item(widget);
    }

    j = ClientUtils::get_list_group_like_blocked(text);

    for (json & item : j)
    {
        std::string name = item["name"];
        std::string id = item["id"];

        GtkWidget * widget = panel->create_search_result_item_group(name, nullptr, id);
        panel->add_search_result_item(widget);
    }

    panel->refresh_search_result();
}

void AddFriendPanel::clear_search_result() {
    GList * list = gtk_container_get_children(GTK_CONTAINER(this->search_result));
    while (list)
    {
        gtk_container_remove(GTK_CONTAINER(this->search_result), (GtkWidget *)list->data);
        list = list->next;
    }
}

void AddFriendPanel::add_search_result_item(GtkWidget *widget) {
    gtk_box_pack_start(GTK_BOX(this->search_result), widget, FALSE, FALSE, 0);
}

void AddFriendPanel::refresh_search_result() {
    gtk_widget_show_all(this->search_result);
}

GtkWidget* AddFriendPanel::create_search_result_item_friend(const std::string &name, GdkPixbuf * pix) {
    GtkWidget * box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    Utils::add_css_class(box, "search_result_item_friend");


    if (pix == nullptr)
    {
        pix = gdk_pixbuf_new_from_file_at_size("../assets/test.jpg", 32, 32, nullptr);
        pix = Utils::get_clipped_image(pix, 32);
    }
    GtkWidget * image = gtk_image_new_from_pixbuf(pix);
    Utils::add_css_class(image, "search_result_image");

    gtk_box_pack_start(GTK_BOX(box), image, FALSE, FALSE, 0);

    GtkWidget * label = gtk_label_new(("用户：" + name).c_str());
    Utils::add_css_class(label, "search_result_label");

    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

    GtkWidget * button = gtk_button_new_with_label("添加好友");
    Utils::add_css_class(button, "search_result_button");
    Data * data = new Data;
    data->panel = this;
    data->type = "friend";
    data->id = name;
    g_signal_connect(button, "clicked", G_CALLBACK(AddFriendPanel::on_button_add_friend_clicked), data);

    gtk_box_pack_end(GTK_BOX(box), button, FALSE, FALSE, 0);

    return box;
}

GtkWidget* AddFriendPanel::create_search_result_item_group(const std::string &name, GdkPixbuf *pix,
                                                           const std::string &id) {
    GtkWidget * box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    Utils::add_css_class(box, "search_result_item_group");


    if (pix == nullptr)
    {
        pix = gdk_pixbuf_new_from_file_at_size("../assets/test.jpg", 32, 32, nullptr);
        pix = Utils::get_clipped_image(pix, 32);
    }
    GtkWidget * image = gtk_image_new_from_pixbuf(pix);
    Utils::add_css_class(image, "search_result_image");

    gtk_box_pack_start(GTK_BOX(box), image, FALSE, FALSE, 0);

    GtkWidget * label = gtk_label_new(("群聊：" + name + "    编号：" + id).c_str());
    Utils::add_css_class(label, "search_result_label");

    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

    GtkWidget * button = gtk_button_new_with_label("申请加群");
    Utils::add_css_class(button, "search_result_button");
    Data * data = new Data;
    data->panel = this;
    data->type = "group";
    data->id = id;
    g_signal_connect(button, "clicked", G_CALLBACK(AddFriendPanel::on_button_add_friend_clicked), data);

    gtk_box_pack_end(GTK_BOX(box), button, FALSE, FALSE, 0);

    return box;
}

void AddFriendPanel::on_button_add_friend_clicked(GtkWidget *widget, gpointer user_data) {
    auto data = (Data *)user_data;
    auto panel = data->panel;

    if (data->type == "friend")
    {
//        ClientUtils::request_add_friend(DataHub::getIns()->username, data->id);
        ClientUtils::send_add_friend_request(DataHub::getIns()->username, data->id);
    }
    else if (data->type == "group")
    {
        ClientUtils::request_join_group(DataHub::getIns()->username, data->id);
    }

    GtkWidget * prev = Utils::find_child(panel->root, "status_bar");
    if (prev) {
        gtk_widget_destroy(prev);
    }

    GtkWidget * status_bar = gtk_label_new("request sent, ikun is waiting");
    gtk_widget_set_name(status_bar, "status_bar");
    gtk_box_pack_end(GTK_BOX(panel->root), status_bar, FALSE, FALSE, 0);

    gtk_widget_show_all(status_bar);

    g_timeout_add(5000, [] (gpointer data) ->gboolean {
        auto lbl = (GtkWidget *)data;

        if (GTK_IS_WIDGET(lbl)) {
            gtk_widget_destroy(lbl);
        }

//        p->status_bar = nullptr;


        return false;
        }, status_bar);
}