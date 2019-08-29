//
// Created by iatow on 19-8-28.
//

#ifndef KUNKUN_USERPANEL_H
#define KUNKUN_USERPANEL_H


class UserPanel {
private:

    GtkWidget * root_scroll = nullptr;
    GtkWidget * root = nullptr;

    UserPanel();

public:
    static UserPanel * create();

    GtkWidget * widget();
};


#endif //KUNKUN_USERPANEL_H
