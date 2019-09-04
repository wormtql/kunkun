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

GtkWidget* Utils::create_button_with_icon(const std::string &file, int size) {
    GdkPixbuf * pix = gdk_pixbuf_new_from_file_at_size(file.c_str(), size, size, nullptr);

    GtkWidget * image = gtk_image_new_from_pixbuf(pix);

    GtkWidget * button = gtk_button_new();

    gtk_container_add(GTK_CONTAINER(button), image);

    return button;
}

GtkWidget* Utils::create_image_from_file_at_size(const std::string &file, int size) {
    GdkPixbuf * pix;
    if (size <= 0) {
        pix = gdk_pixbuf_new_from_file(file.c_str(), nullptr);
    } else {
        pix = gdk_pixbuf_new_from_file_at_size(file.c_str(), size, size, nullptr);

    }

    return gtk_image_new_from_pixbuf(pix);
}

std::string Utils::get_filename_from_path(const std::string &path) {
    std::string temp;

    for (int i = (int)path.size() - 1; i >= 0; i--) {
        if (path[i] == '\\' || path[i] == '/') {
            break;
        }

        temp.push_back(path[i]);
    }

    std::reverse(temp.begin(), temp.end());

    return temp;
}

GtkWidget* Utils::create_image_from_file_at_size(const std::string &file, int width, int height) {
    GdkPixbuf * pix;
    if (height <= 0 || width <= 0) {
        pix = gdk_pixbuf_new_from_file(file.c_str(), nullptr);
    } else {
        pix = gdk_pixbuf_new_from_file_at_size(file.c_str(), width, height, nullptr);

    }

    return gtk_image_new_from_pixbuf(pix);
}

GdkPixbuf* Utils::create_round_dot_pix(int size, int r, int g, int b) {
    cairo_surface_t * surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, size, size);
    cairo_t * cr = cairo_create(surface);


    cairo_set_source_rgb(cr, r / 255.0, g / 255.0, b / 255.0);
    cairo_arc(cr, size / 2, size / 2, size / 2, 0, 2 * M_PI);
    cairo_fill(cr);

    return gdk_pixbuf_get_from_surface(surface, 0, 0, size, size);
}

int Utils::parse_recv_file(const char *buf, json &recv_msg) {
    int len = 0;
    int flag = 0;
    for(len = 0; ; len++ )
    {
        if( buf[len] == '{' ) flag ++;
        else if( buf[len] == '}' ) flag --;
        if( !flag ) break;
    }
    len++;
//    char tmp[20000];
//    char tp = buf[len];
//    buf[len] = "\0";
//    buf[len] = '\0';
//    strcpy(tmp, buf);
//    buf[len] = tp;
//    recv_msg = json::parse( tmp );

    recv_msg = json::parse(buf, buf + len);

    return len;
}

int Utils::check_valid_string(const std::string &text) {
    bool flag = true;
    for (const char c : text) {
        bool valid = (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
        if (valid == false) {
            flag = false;
            break;
        }
    }

    return flag;
}