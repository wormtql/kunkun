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

    static GdkPixbuf * load_from_file(const std::string & file, int size);

    static GtkWidget * create_button_with_icon(str file, int size);

    static GtkWidget * create_image_from_file_at_size(str file, int size);

    static GtkWidget * create_image_from_file_at_size(str file, int width, int height);

    static std::string get_filename_from_path(str path);

    static GdkPixbuf * create_round_dot_pix(int size, int r, int g, int b);

    static int parse_recv_file(const char *buf, json &recv_msg);

    static int check_valid_string(str text);
};


#endif //HOMEWORK_UTILS_H
