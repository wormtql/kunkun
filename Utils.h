//
// Created by iatow on 19-8-27.
//

#ifndef HOMEWORK_UTILS_H
#define HOMEWORK_UTILS_H


class Utils {
public:
    static void set_css_id(GtkWidget * widget, const char * name);

    static GtkWidget * find_child(GtkWidget * parent, const char * name);
};


#endif //HOMEWORK_UTILS_H
