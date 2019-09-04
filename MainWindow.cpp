//
// Created by iatow on 19-8-28.
//

#include "INCLUDE.h"

#include "MainWindow.h"

using std::string;
using std::cout;
using std::ofstream;

MainWindow * MainWindow::create() {
    return new MainWindow();
}

MainWindow::MainWindow() {
    this->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_decorated(GTK_WINDOW(this->window), FALSE);
    gtk_window_set_default_size(GTK_WINDOW(this->window), 900, 600);
    gtk_widget_set_name(window, "main_window");

    gtk_widget_set_events(this->window, GDK_BUTTON_PRESS_MASK | GDK_BUTTON1_MOTION_MASK);

//    g_signal_connect(this->top_bar, "button_press_event", G_CALLBACK(LoginWindow::button_press_event), this);
    g_signal_connect(this->window, "button_press_event", G_CALLBACK(MainWindow::button_press_event), this);
//    g_signal_connect(this->top_bar, "button_release_event", G_CALLBACK(LoginWindow::button_release_event), this);
    g_signal_connect(this->window, "button_release_event", G_CALLBACK(MainWindow::button_release_event), this);
//    g_signal_connect(this->top_bar, "motion_notify_event", G_CALLBACK(LoginWindow::motion_notify_event), this);
    g_signal_connect(this->window, "motion_notify_event", G_CALLBACK(MainWindow::motion_notify_event), this);

    GtkWidget * root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), root);

    this->top_bar = create_top_bar();
    gtk_box_pack_start(GTK_BOX(root), top_bar, FALSE, FALSE, 0);

    this->body = create_body();
    gtk_box_pack_start(GTK_BOX(root), body, TRUE, TRUE, 0);

    MainWindow::on_button_user_clicked(nullptr, this);

    GMainContext * context = g_main_context_get_thread_default();

    Thread::set_call_back("recv_file", [this, context] (char * data, int size) {

        json j;

        int jsize = Utils::parse_recv_file(data, j);

        cout << "file recv\n";
        cout << j << std::endl;

        std::string filename = j["filename"];
        std::string fileid = j["fileid"];
        bool eof = j["eof"];

        ofstream file("files/" + fileid + "_" + filename, std::ios::app | std::ios::binary);
        file.write(data + jsize, size - jsize);
        file.close();
        printf("size:%d %d\n", size, jsize);
        if (eof) {

            Data * da = new Data;
            da->context = context;
            da->window = this;
            da->msg = "文件：" + (string)j["filename"] + " 接收完成";

            g_main_context_invoke(context, [] (gpointer user_data) -> gboolean {

                auto d = (Data *)user_data;
                auto window = d->window;

                GtkWidget * prev = Utils::find_child(window->top_bar, "top_label");
                if (prev) {
                    gtk_widget_destroy(prev);
                }

                GtkWidget * lbl = gtk_label_new(d->msg.c_str());
                gtk_widget_set_name(lbl, "top_label");

                gtk_box_pack_start(GTK_BOX(window->top_bar), lbl, TRUE, TRUE, 0);
                gtk_widget_show_all(lbl);

                g_timeout_add(5000, [] (gpointer data) -> gboolean {


                    if (GTK_IS_WIDGET(data)) {
                        gtk_widget_destroy((GtkWidget *)data);
                    }

                    return false;
                }, lbl);

                return false;

            }, da);
        }

    });

//    Data * data = new Data;
//    data->context = context;
//    data->widget = lbl;

    Thread::set_call_back("recv_sys_msg", [this, context] (char * da, int size) {

        std::string data = da;

        json j = json::parse(data);

        cout << "sys msg: " + (string)j["msg"] << "\n";


        Data * d = new Data;
        d->msg = "系统消息：" + (string)j["msg"];
        d->window = this;

        g_main_context_invoke(context, [] (gpointer user_data) -> gboolean {

            printf("invoked\n");

            auto d = (Data *)user_data;
            auto window = d->window;

            GtkWidget * prev = Utils::find_child(window->top_bar, "top_label");
            if (prev) {
                gtk_widget_destroy(prev);
            }

            GtkWidget * lbl = gtk_label_new(d->msg.c_str());
//            GtkWidget
            gtk_widget_set_name(lbl, "top_label");

            gtk_box_pack_start(GTK_BOX(window->top_bar), lbl, TRUE, TRUE, 0);
            gtk_widget_show_all(lbl);

            g_timeout_add(5000, [] (gpointer data) -> gboolean {

                auto w = (GtkWidget *)data;

                if (GTK_IS_WIDGET(w)) {
                    gtk_widget_destroy(w);
                }

                return false;
            }, lbl);

            return false;

        }, d);



    });
}


/*
 * function: create left most functional button
 *
 */
GtkWidget* MainWindow::create_side_bar() {
    side_bar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_widget_set_name(side_bar, "toolbar");


    // button user
//    GtkWidget * button_user = gtk_button_new_from_icon_name("go-bottom", GTK_ICON_SIZE_BUTTON);
    GtkWidget * button_user = Utils::create_button_with_icon("../assets/tag.png", 28);
    g_signal_connect(button_user, "clicked", G_CALLBACK(MainWindow::on_button_user_clicked), this);
    gtk_widget_set_name(button_user, "main_window_button_user");


    // button user
//    GtkWidget * button_friend = gtk_button_new_from_icon_name("go-bottom", GTK_ICON_SIZE_BUTTON);
    GtkWidget * button_friend = Utils::create_button_with_icon("../assets/msg.png", 28);
    g_signal_connect(button_friend, "clicked", G_CALLBACK(MainWindow::on_button_friend_clicked), this);
    gtk_widget_set_name(button_friend, "main_window_button_friend");


    // button add friend
//    GtkWidget * button_add_friend = gtk_button_new_from_icon_name("go-bottom", GTK_ICON_SIZE_BUTTON);
    GtkWidget * button_add_friend = Utils::create_button_with_icon("../assets/info.png", 28);
    g_signal_connect(button_add_friend, "clicked", G_CALLBACK(MainWindow::on_button_add_friend_clicked), this);
    gtk_widget_set_name(button_add_friend, "main_window_button_add_friend");


    // button console
//    GtkWidget * button_console = gtk_button_new_from_icon_name("go-bottom", GTK_ICON_SIZE_BUTTON);
    GtkWidget * button_console = Utils::create_button_with_icon("../assets/console.png", 24);
    g_signal_connect(button_console, "clicked", G_CALLBACK(MainWindow::on_button_console_clicked), this);
    gtk_widget_set_name(button_console, "main_window_button_console");


    gtk_box_pack_start(GTK_BOX(side_bar), button_user, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(side_bar), button_friend, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(side_bar), button_add_friend, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(side_bar), button_console, FALSE, FALSE, 0);

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
        if (window->chat_panel != nullptr && window->current_page == window->chat_panel->widget()) {
            return;
        }
        gtk_widget_hide(window->current_page);
    }

    if (!window->chat_panel) {
        window->chat_panel = ChatPanel::create();
        window->chat_panel->set_parent_window(window->window);
        gtk_box_pack_start(GTK_BOX(window->body), window->chat_panel->widget(), TRUE, TRUE, 0);
    }

    json friends = ClientUtils::get_friends(DataHub::getIns()->username);
    json groups = ClientUtils::get_groups(DataHub::getIns()->username);

    window->chat_panel->refresh_friends_list(friends, groups);
    window->chat_panel->mark_all_new_msg();

    window->current_page = window->chat_panel->widget();
    gtk_widget_show_all(window->current_page);
}

void MainWindow::on_button_user_clicked(GtkWidget *widget, gpointer data) {
    auto window = (MainWindow *)data;

    if (window->current_page) {
        if (window->user_panel != nullptr && window->current_page == window->user_panel->widget()) {
            return;
        }
        gtk_widget_hide(window->current_page);
    }

    if (!window->user_panel) {
        window->user_panel = UserPanel::create();
        gtk_box_pack_start(GTK_BOX(window->body), window->user_panel->widget(), TRUE, TRUE, 0);
    }

    window->user_panel->refresh_user_info();

    window->current_page = window->user_panel->widget();
    gtk_widget_show_all(window->current_page);
}

void MainWindow::on_button_add_friend_clicked(GtkWidget *widget, gpointer data) {
    auto window = (MainWindow *)data;

    if (window->current_page) {
        if (window->add_friend_panel != nullptr && window->current_page == window->add_friend_panel->widget()) {
            return;
        }
        gtk_widget_hide(window->current_page);
    }

    if (!window->add_friend_panel) {
        window->add_friend_panel = AddFriendPanel::create();
        gtk_box_pack_start(GTK_BOX(window->body), window->add_friend_panel->widget(), TRUE, TRUE, 0);
    }

    window->current_page = window->add_friend_panel->widget();
    gtk_widget_show_all(window->current_page);
}

void MainWindow::on_button_console_clicked(GtkWidget *widget, gpointer data) {
    auto window = (MainWindow *)data;

    if (window->current_page) {
        if (window->console_panel != nullptr && window->current_page == window->console_panel->widget()) {
            return;
        }
        gtk_widget_hide(window->current_page);
    }

    if (!window->console_panel) {
        window->console_panel = ConsolePanel::create();
        gtk_box_pack_start(GTK_BOX(window->body), window->console_panel->widget(), TRUE, TRUE, 0);
    }

    window->current_page = window->console_panel->widget();
    gtk_widget_show_all(window->current_page);
}

gint MainWindow::button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer data) {
//    printf("worm\n");

    auto window = (MainWindow *)data;

    window->x = (int)event->x;
    window->y = (int)event->y;
    window->drag = true;

    return TRUE;
}

gint MainWindow::button_release_event(GtkWidget *widget, GdkEventButton *event, gpointer data) {

//    printf("release\n");

    auto window = (MainWindow *)data;

    window->drag = false;

    return TRUE;
}

gint MainWindow::motion_notify_event(GtkWidget *widget, GdkEventButton *event, gpointer data) {

//    printf("motion\n");

    auto window = (MainWindow *)data;
    if (window->drag) {

//        printf("aaa\n");

        int x, y;
        gtk_window_get_position(GTK_WINDOW(window->window), &x, &y);

        gtk_window_move(GTK_WINDOW(window->window), x + event->x - window->x, y + event->y - window->y);
//        window->x = event->x;
//        window->y = event->y;
    }

    return TRUE;
}