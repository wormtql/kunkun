//
// Created by iatow on 19-8-28.
//

#include "INCLUDE.h"

#include "MainWindow.h"

MainWindow * MainWindow::create() {
    return new MainWindow();
}

MainWindow::MainWindow() {
    this->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_decorated(GTK_WINDOW(this->window), FALSE);
    gtk_window_set_default_size(GTK_WINDOW(this->window), 900, 600);
    gtk_widget_set_name(window, "main_window");

    GtkWidget * root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), root);

    this->top_bar = create_top_bar();
    gtk_box_pack_start(GTK_BOX(root), top_bar, FALSE, FALSE, 0);

    this->body = create_body();
    gtk_box_pack_start(GTK_BOX(root), body, TRUE, TRUE, 0);

}


/*
 * function: create left most functional button
 *
 */
GtkWidget* MainWindow::create_side_bar() {
    side_bar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(side_bar, "toolbar");


    // button user
    GtkWidget * button_user = gtk_button_new_from_icon_name("go-bottom", GTK_ICON_SIZE_BUTTON);
    g_signal_connect(button_user, "clicked", G_CALLBACK(MainWindow::on_button_user_clicked), this);
    gtk_widget_set_name(button_user, "main_window_button_user");


    // button user
    GtkWidget * button_friend = gtk_button_new_from_icon_name("go-bottom", GTK_ICON_SIZE_BUTTON);
    g_signal_connect(button_friend, "clicked", G_CALLBACK(MainWindow::on_button_friend_clicked), this);
    gtk_widget_set_name(button_friend, "main_window_button_friend");


    // button add friend
    GtkWidget * button_add_friend = gtk_button_new_from_icon_name("go-bottom", GTK_ICON_SIZE_BUTTON);
    g_signal_connect(button_add_friend, "clicked", G_CALLBACK(MainWindow::on_button_friend_clicked), this);
    gtk_widget_set_name(button_add_friend, "main_window_button_add_friend");


    gtk_box_pack_start(GTK_BOX(side_bar), button_user, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(side_bar), button_friend, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(side_bar), button_add_friend, FALSE, FALSE, 0);

    return side_bar;
}

GtkWidget* MainWindow::create_top_bar() {
    top_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(top_bar, "main_window_top_bar");

    GtkWidget * button_close = gtk_button_new_with_label("x");
    gtk_widget_set_name(button_close, "main_window_button_close");
    g_signal_connect_swapped(button_close, "clicked", G_CALLBACK(gtk_widget_destroy), this->window);

    GtkWidget * label = gtk_label_new("kunkun");
    gtk_widget_set_name(label, "main_window_label_caption");

    gtk_box_pack_start(GTK_BOX(top_bar), label, FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(top_bar), button_close, FALSE, FALSE, 0);

    return top_bar;
}

GtkWidget * MainWindow::create_body() {
    this->body = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(body, "main_window_body");

    this->side_bar = create_side_bar();
    gtk_box_pack_start(GTK_BOX(body), side_bar, FALSE, FALSE, 0);

    return body;
}

GtkWidget* MainWindow::widget() {
    return this->window;
}

void MainWindow::on_button_friend_clicked(GtkWidget *widget, gpointer data) {
    auto window = (MainWindow *)data;

    if (window->current_page) {
        gtk_widget_hide(window->current_page);
    }

    if (!window->chat_panel) {
        window->chat_panel = ChatPanel::create();
        gtk_box_pack_start(GTK_BOX(window->body), window->chat_panel->widget(), TRUE, TRUE, 0);
    }

    window->current_page = window->chat_panel->widget();
    gtk_widget_show_all(window->current_page);
}

void MainWindow::on_button_user_clicked(GtkWidget *widget, gpointer data) {
    auto window = (MainWindow *)data;

    if (window->current_page) {
        gtk_widget_hide(window->current_page);
    }

    if (!window->user_panel) {
        window->user_panel = UserPanel::create();
        gtk_box_pack_start(GTK_BOX(window->body), window->user_panel->widget(), TRUE, TRUE, 0);
    }

    window->current_page = window->user_panel->widget();
    gtk_widget_show_all(window->current_page);
}

void MainWindow::on_button_add_friend_clicked(GtkWidget *widget, gpointer data) {

}

void MainWindow::on_button_console_clicked(GtkWidget *widget, gpointer data) {

}