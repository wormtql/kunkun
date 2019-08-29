//
// Created by iatow on 19-8-28.
//

#include "INCLUDE.h"

#include "UserPanel.h"


UserPanel::UserPanel() {

    root_scroll = gtk_scrolled_window_new(nullptr, nullptr);

    root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(root, "user_panel");
    gtk_container_add(GTK_CONTAINER(root_scroll), root);


}

UserPanel * UserPanel::create() {
    return new UserPanel;
}

GtkWidget* UserPanel::widget() {
    return root_scroll;
}