//
// Created by iatow on 19-8-27.
//

#include "INCLUDE.h"

void Utils::set_css_id(GtkWidget *widget, const char *name) {
    gtk_widget_set_name(widget, name);
}

GtkWidget * Utils::find_child(GtkWidget *parent, const char *name) {

    const char * temp = gtk_widget_get_name(parent);

    if (strcmp(temp, name) == 0) {
        return parent;
    }

    if (GTK_IS_BIN(parent)) {
        GtkWidget *child = gtk_bin_get_child(GTK_BIN(parent));
        return find_child(child, name);
    }

    if (GTK_IS_CONTAINER(parent)) {
        GList *children = gtk_container_get_children(GTK_CONTAINER(parent));
        while (children != nullptr) {
            GtkWidget* widget = find_child((GtkWidget *)children->data, name);
            if (widget != nullptr) {
                return widget;
            }

            children = g_list_next(children);
        }
    }

    return nullptr;
}