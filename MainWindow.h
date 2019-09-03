//
// Created by iatow on 19-8-28.
//

#ifndef HOMEWORK_MINWINDOW_H
#define HOMEWORK_MINWINDOW_H


class MainWindow {
private:
    struct Data {
        MainWindow * window;
        std::string msg;
        GtkWidget * widget;
        GMainContext * context;
    };
private:
    GtkWidget * window = nullptr;

    GtkWidget * top_bar = nullptr;
    GtkWidget * body = nullptr;

    GtkWidget * side_bar = nullptr;

    GtkWidget * current_page = nullptr;

    ChatPanel * chat_panel = nullptr;
    UserPanel * user_panel = nullptr;
    AddFriendPanel * add_friend_panel = nullptr;
    ConsolePanel * console_panel = nullptr;


    MainWindow();

    GtkWidget * create_side_bar();

    GtkWidget * create_top_bar();

    GtkWidget * create_body();

    static void on_button_user_clicked(GtkWidget * widget, gpointer data);

    static void on_button_friend_clicked(GtkWidget * widget, gpointer data);

    static void on_button_add_friend_clicked(GtkWidget * widget, gpointer data);

    static void on_button_console_clicked(GtkWidget * widget, gpointer data);


    int x, y;
    int drag = false;
    static gint button_press_event(GtkWidget * widget, GdkEventButton * event, gpointer data);

    static gint button_release_event(GtkWidget * widget, GdkEventButton * event, gpointer data);

    static gint motion_notify_event(GtkWidget * widget, GdkEventButton * event, gpointer data);

public:
    static MainWindow * create();

    GtkWidget * widget();
};


#endif //HOMEWORK_MINWINDOW_H
