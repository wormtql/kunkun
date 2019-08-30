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


    GtkWidget * entry = gtk_entry_new();
    Utils::add_css_class(entry, "user_panel_entry");
    g_signal_connect(entry, "activate", G_CALLBACK(UserPanel::on_user_name_enter), this);
    g_signal_connect(entry, "focus-out-event", G_CALLBACK(UserPanel::on_user_name_enter), this);
    gtk_box_pack_end(GTK_BOX(box), entry, FALSE, FALSE, 0);

    return box;
}

GtkWidget* UserPanel::create_user_sign_field() {
    GtkWidget * box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    Utils::add_css_class(box, "user_panel_field_box");

    GtkWidget * label = gtk_label_new("sign: ");
    Utils::add_css_class(label, "user_panel_label");
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

    GtkWidget * entry = gtk_entry_new();
    Utils::add_css_class(entry, "user_panel_entry");
    g_signal_connect(entry, "activate", G_CALLBACK(UserPanel::on_sign_enter), this);
    gtk_box_pack_end(GTK_BOX(box), entry, FALSE, FALSE, 0);

    return box;
}

GtkWidget* UserPanel::create_gender_field() {
    GtkWidget * box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    Utils::add_css_class(box, "user_panel_field_box");

    GtkWidget * label = gtk_label_new("gender: ");
    Utils::add_css_class(label, "user_panel_label");
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);

    GtkWidget * entry = gtk_entry_new();
    Utils::add_css_class(entry, "user_panel_entry");
    g_signal_connect(entry, "activate", G_CALLBACK(UserPanel::on_gender_enter), this);
    gtk_box_pack_end(GTK_BOX(box), entry, FALSE, FALSE, 0);

    return box;
}

int UserPanel::on_user_name_enter(GtkWidget *widget, gpointer data) {
    printf("enter\n");

    return GDK_EVENT_PROPAGATE;
}

int UserPanel::on_sign_enter(GtkWidget *widget, gpointer data) {
    return GDK_EVENT_PROPAGATE;
}

int UserPanel::on_gender_enter(GtkWidget *widget, gpointer data) {
    return GDK_EVENT_PROPAGATE;
}