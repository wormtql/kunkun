//
// Created by iatow on 19-8-28.
//

#include "INCLUDE.h"

#include "UserPanel.h"


UserPanel::UserPanel() {

    root_scroll = gtk_scrolled_window_new(nullptr, nullptr);

    root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(root_scroll, "user_panel");
    gtk_container_add(GTK_CONTAINER(root_scroll), root);

    GdkPixbuf * temp_pix = gdk_pixbuf_new_from_file_at_size("../assets/test.jpg", 128, 128, nullptr);
    avatar = gtk_image_new_from_pixbuf(Utils::get_clipped_image(temp_pix, 128));
    gtk_widget_set_name(avatar, "avatar");

    gtk_box_pack_start(GTK_BOX(root), avatar, FALSE, FALSE, 0);

    GtkWidget * temp = create_user_name_field();
    gtk_box_pack_start(GTK_BOX(root), temp, FALSE, FALSE, 0);

    temp = create_user_sign_field();
    gtk_box_pack_start(GTK_BOX(root), temp, FALSE, FALSE, 0);

    temp = create_gender_field();
    gtk_box_pack_start(GTK_BOX(root), temp, FALSE, FALSE, 0);


}

UserPanel * UserPanel::create() {
    return new UserPanel;
}

GtkWidget* UserPanel::widget() {
    return root_scroll;
}

GtkWidget* UserPanel::create_user_name_field() {
    GtkWidget * box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    Utils::add_css_class(box, "user_panel_field_box");

    GtkWidget * label = gtk_label_new("user name: ");
    Utils::add_css_class(label, "user_panel_label");
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);


    entry_username = gtk_entry_new();
    Utils::add_css_class(entry_username, "user_panel_entry");
    g_signal_connect(entry_username, "activate", G_CALLBACK(UserPanel::on_user_name_enter), this);
    g_signal_connect(entry_username, "focus-out-event", G_CALLBACK(UserPanel::on_user_name_enter), this);
    gtk_box_pack_end(GTK_BOX(box), entry_username, FALSE, FALSE, 0);

    return box;
}

GtkWidget* UserPanel::create_user_sign_field() {
    GtkWidget * box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    Utils::add_css_class(box, "user_panel_field_box");

    GtkWidget * label = gtk_label_new("sign: ");
    Utils::add_css_class(label, "user_panel_label");
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

    entry_sign = gtk_entry_new();
    Utils::add_css_class(entry_sign, "user_panel_entry");
    g_signal_connect(entry_sign, "activate", G_CALLBACK(UserPanel::on_sign_enter), this);
    g_signal_connect(entry_sign, "focus-out-event", G_CALLBACK(UserPanel::on_sign_enter), this);
    gtk_box_pack_end(GTK_BOX(box), entry_sign, FALSE, FALSE, 0);

    return box;
}

GtkWidget* UserPanel::create_gender_field() {
    GtkWidget * box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    Utils::add_css_class(box, "user_panel_field_box");

    GtkWidget * label = gtk_label_new("gender: ");
    Utils::add_css_class(label, "user_panel_label");
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

    entry_gender = gtk_entry_new();
    Utils::add_css_class(entry_gender, "user_panel_entry");
    g_signal_connect(entry_gender, "activate", G_CALLBACK(UserPanel::on_gender_enter), this);
    g_signal_connect(entry_gender, "focus-out-event", G_CALLBACK(UserPanel::on_gender_enter), this);
    gtk_box_pack_end(GTK_BOX(box), entry_gender, FALSE, FALSE, 0);

    return box;
}

int UserPanel::on_user_name_enter(GtkWidget *widget, gpointer data) {
    auto panel = (UserPanel *)data;

    if (GTK_IS_ENTRY(panel->entry_username)) {
        std::string val = gtk_entry_get_text(GTK_ENTRY(panel->entry_username));
        ClientUtils::alter_user_info(DataHub::getIns()->username, "username", val);
    }

    return GDK_EVENT_PROPAGATE;
}

int UserPanel::on_sign_enter(GtkWidget *widget, gpointer data) {
    auto panel = (UserPanel *)data;

    if (GTK_IS_ENTRY(panel->entry_sign)) {
        std::string val = gtk_entry_get_text(GTK_ENTRY(panel->entry_sign));
        ClientUtils::alter_user_info(DataHub::getIns()->username, "sign", val);

    }

    return GDK_EVENT_PROPAGATE;
}

int UserPanel::on_gender_enter(GtkWidget *widget, gpointer data) {
    auto panel = (UserPanel *)data;
//    auto entry = (GtkWidget *)data;

    if (GTK_IS_ENTRY(panel->entry_gender)) {
        std::string val = gtk_entry_get_text(GTK_ENTRY(panel->entry_gender));
        ClientUtils::alter_user_info(DataHub::getIns()->username, "gender", val);

    }

//    std::string val = gtk_entry_get_text(GTK_ENTRY(entry));
//    ClientUtils::alter_user_info(DataHub::getIns()->username, "gender", val);

    return GDK_EVENT_PROPAGATE;
}

void UserPanel::refresh_user_info() {
    std::string val;

    val = ClientUtils::get_user_info(DataHub::getIns()->username, "username");
    gtk_entry_set_text(GTK_ENTRY(this->entry_username), val.c_str());

    val = ClientUtils::get_user_info(DataHub::getIns()->username, "sign");
    gtk_entry_set_text(GTK_ENTRY(this->entry_sign), val.c_str());

    val = ClientUtils::get_user_info(DataHub::getIns()->username, "gender");
    gtk_entry_set_text(GTK_ENTRY(this->entry_gender), val.c_str());
}