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

    GtkWidget * login_form;
    GtkWidget * signup_form;

    GtkWidget * button_login_switch;
    GtkWidget * button_signup_switch;

    GtkWidget * label_error;


    GtkWidget * create_field(const char * icon_name, const char * icon_id, const char * input_ph, const char * input_id);

    GtkWidget * create_login_form();

    GtkWidget * create_signup_form();

    static void btn_login_switch_clicked(GtkWidget * widget, gpointer data);

    static void btn_signup_switch_clicked(GtkWidget * widget, gpointer data);

    static void btn_login_clicked(GtkWidget * widget, gpointer data);

    static void btn_signup_clicked(GtkWidget * widget, gpointer data);

public:
    LoginWindow();


    GtkWidget * widget();

    void set_error_info(const char * info);
};

#endif //HOMEWORK_LOGIN_WINDOW_H
