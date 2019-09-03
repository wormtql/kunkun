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
//    GtkWidget * entry_username;

    GtkWidget * username_label = nullptr;

    GtkWidget * entry_sign;
//    GtkWidget * entry_gender;

    GtkWidget * radio_male;
    GtkWidget * radio_female;
    GtkWidget * radio_secret;

    UserPanel();

    GtkWidget * create_user_name_field();
    GtkWidget * create_user_sign_field();
    GtkWidget * create_gender_field();

    static int on_user_name_enter(GtkWidget * widget, gpointer data);

    static int on_sign_enter(GtkWidget * widget, gpointer data);

    static int on_gender_radio_clicked(GtkWidget * widget, gpointer data);


public:
    static UserPanel * create();

    GtkWidget * widget();


    void refresh_user_info();
};


#endif //KUNKUN_USERPANEL_H
