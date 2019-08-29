//
// Created by iatow on 19-8-27.
//

#ifndef HOMEWORK_UTILS_H
#define HOMEWORK_UTILS_H


class Utils {
public:
    static void set_css_id(GtkWidget * widget, const char * name);

    static GtkWidget * find_child(GtkWidget * parent, const char * name);

    static void add_css_class(GtkWidget * widget, const char * name);

    static void remove_css_class(GtkWidget * widget, const char * name);

    static GdkPixbuf * get_clipped_image(GdkPixbuf * image, int radius);
};


#endif //HOMEWORK_UTILS_H
