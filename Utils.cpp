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

void Utils::add_css_class(GtkWidget *widget, const char *name) {
    GtkStyleContext * context = gtk_widget_get_style_context(widget);

    gtk_style_context_add_class(context, name);
}

void Utils::remove_css_class(GtkWidget *widget, const char *name) {
    GtkStyleContext * context = gtk_widget_get_style_context(widget);

    gtk_style_context_remove_class(context, name);
}

GdkPixbuf * Utils::get_clipped_image(GdkPixbuf *image, int radius) {
    cairo_surface_t * surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, radius, radius);
    cairo_t * cr = cairo_create(surface);

    int height = gdk_pixbuf_get_height(image);
    int width = gdk_pixbuf_get_width(image);

    gdk_cairo_set_source_pixbuf(cr, image, 0, 0);
    cairo_arc(cr, radius / 2.0, radius / 2.0, radius / 2.0, 0, 2 * M_PI);
    cairo_clip(cr);
    cairo_paint(cr);

    return gdk_pixbuf_get_from_surface(surface, 0, 0, radius, radius);
}

GdkPixbuf* Utils::load_from_file(const std::string &file, int size) {
    if (size <= 0)
    {
        return gdk_pixbuf_new_from_file(file.c_str(), nullptr);
    }
    else
    {
        return gdk_pixbuf_new_from_file_at_size(file.c_str(), size, size, nullptr);
    }
}