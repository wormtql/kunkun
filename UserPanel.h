//
// Created by iatow on 19-8-28.
//

#ifndef KUNKUN_USERPANEL_H
#define KUNKUN_USERPANEL_H


class UserPanel {
private:

    GtkWidget * root_scroll = nullptr;
    GtkWidget * root = nullptr;

    GtkWidget * avatar;
    GtkWidget * label_user_name;

    UserPanel();

    GtkWidget * create_user_name_field();
    GtkWidget * create_user_sign_field();
    GtkWidget * create_gender_field();

    static void on_user_name_enter(GtkWidget * widget, gpointer data);

    static void on_sign_enter(GtkWidget * widget, gpointer data);

    static void on_gender_enter(GtkWidget * widget, gpointer data);


public:
    static UserPanel * create();

    GtkWidget * widget();
};


#endif //KUNKUN_USERPANEL_H
