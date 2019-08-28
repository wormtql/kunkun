//
// Created by iatow on 19-8-28.
//

#ifndef HOMEWORK_MINWINDOW_H
#define HOMEWORK_MINWINDOW_H


class MainWindow {
private:
    GtkWidget * window = nullptr;

    GtkWidget * top_bar = nullptr;
    GtkWidget * body = nullptr;

    GtkWidget * side_bar = nullptr;

    GtkWidget * current_page = nullptr;

    ChatPanel * chat_panel = nullptr;


    MainWindow();

    GtkWidget * create_side_bar();

    GtkWidget * create_top_bar();

    GtkWidget * create_body();

    static void on_button_user_clicked(GtkWidget * widget, gpointer data);

    static void on_button_friend_clicked(GtkWidget * widget, gpointer data);

public:
    static MainWindow * create();

    GtkWidget * widget();
};


#endif //HOMEWORK_MINWINDOW_H
