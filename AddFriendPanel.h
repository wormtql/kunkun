//
// Created by iatow on 19-8-29.
//

#ifndef KUNKUN_ADDFRIENDPANEL_H
#define KUNKUN_ADDFRIENDPANEL_H


class AddFriendPanel {
private:

    GtkWidget * root = nullptr;

    GtkWidget * search_bar = nullptr;

    GtkWidget * search_result = nullptr;

    AddFriendPanel();

public:
    static AddFriendPanel * create();

    GtkWidget * widget();
};


#endif //KUNKUN_ADDFRIENDPANEL_H
