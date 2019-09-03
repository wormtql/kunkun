//
// Created by iatow on 19-8-29.
//

#ifndef KUNKUN_ADDFRIENDPANEL_H
#define KUNKUN_ADDFRIENDPANEL_H


class AddFriendPanel {
private:
    struct Data {
        AddFriendPanel * panel;
        std::string type;
        std::string id;
    };
private:

    // box
    GtkWidget * root = nullptr;

    // search entry
    GtkWidget * search_bar = nullptr;
    GtkEntryBuffer * search_buffer = nullptr;

    GtkWidget * search_result = nullptr;

//    GtkWidget * status_bar = nullptr;

    AddFriendPanel();

    static void on_text_changed(GtkSearchEntry * entry, gpointer data);

    static void on_button_add_friend_clicked(GtkWidget * widget, gpointer data);

    GtkWidget * create_search_result_item_friend(const std::string & name, GdkPixbuf * pix);

    GtkWidget * create_search_result_item_group(const std::string & name, GdkPixbuf * pix, const std::string & id);

public:
    static AddFriendPanel * create();

    GtkWidget * widget();

    void add_search_result_item(GtkWidget * widget);

    void clear_search_result();

    void refresh_search_result();
};


#endif //KUNKUN_ADDFRIENDPANEL_H
