//
// Created by iatow on 19-8-27.
//

#ifndef HOMEWORK_LOGIN_WINDOW_H
#define HOMEWORK_LOGIN_WINDOW_H

class LoginWindow {
private:
    GtkWidget * window = nullptr;

    GtkWidget * close_button = nullptr;
    GtkWidget * top_bar = nullptr;
    GtkWidget * root_box = nullptr;

    GtkWidget * login_form = nullptr;
    GtkWidget * signup_form = nullptr;

    GtkWidget * button_login_switch = nullptr;
    GtkWidget * button_signup_switch = nullptr;

//    GtkWidget * label_error = nullptr;

    std::function<void ()> func;


    LoginWindow();

    GtkWidget * create_field(const char * icon_name, const char * icon_id, const char * input_ph, const char * input_id);

    GtkWidget * create_login_form();

    GtkWidget * create_signup_form();

    static void btn_login_switch_clicked(GtkWidget * widget, gpointer data);

    static void btn_signup_switch_clicked(GtkWidget * widget, gpointer data);

    static void btn_login_clicked(GtkWidget * widget, gpointer data);

    static void btn_signup_clicked(GtkWidget * widget, gpointer data);


    int x, y;
    int drag = false;
    static gint button_press_event(GtkWidget * widget, GdkEventButton * event, gpointer data);

    static gint button_release_event(GtkWidget * widget, GdkEventButton * event, gpointer data);

    static gint motion_notify_event(GtkWidget * widget, GdkEventButton * event, gpointer data);

public:

    static LoginWindow * create();

    GtkWidget * widget();

    void set_error_info(const char * info);

    void set_login_success_callback(std::function<void ()> && func);
};

#endif //HOMEWORK_LOGIN_WINDOW_H
