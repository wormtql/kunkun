//
// Created by iatow on 19-8-27.
//

#include "INCLUDE.h"

LoginWindow::LoginWindow() {
    // window
    this->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_decorated(GTK_WINDOW(this->window), FALSE);
    gtk_widget_set_name(this->window, "login_window");
    gtk_window_set_default_size(GTK_WINDOW(this->window), 500, 700);

    // top bar
    this->top_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(this->top_bar, "top_bar");

    // close button
    this->close_button = gtk_button_new_with_label("x");
    gtk_widget_set_name(this->close_button, "close_button");
    g_signal_connect_swapped(this->close_button, "clicked", G_CALLBACK(gtk_widget_destroy), this->window);

    // login switch button
    this->button_login_switch = gtk_button_new_from_icon_name("user-home", GTK_ICON_SIZE_BUTTON);
    gtk_widget_set_name(this->button_login_switch, "button_login_switch");
    g_signal_connect(this->button_login_switch, "clicked", G_CALLBACK(LoginWindow::btn_login_switch_clicked), this);
    gtk_box_pack_start(GTK_BOX(this->top_bar), this->button_login_switch, FALSE, FALSE, 0);


    // signup switch button
    this->button_signup_switch = gtk_button_new_from_icon_name("user-idle", GTK_ICON_SIZE_BUTTON);
    gtk_widget_set_name(this->button_signup_switch, "button_signup_switch");
    g_signal_connect(this->button_signup_switch, "clicked", G_CALLBACK(LoginWindow::btn_signup_switch_clicked), this);
    gtk_box_pack_start(GTK_BOX(this->top_bar), this->button_signup_switch, FALSE, FALSE, 0);

    // pack close button to top bar
    gtk_box_pack_end(GTK_BOX(this->top_bar), this->close_button, FALSE, FALSE, 0);

    // root box
    this->root_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    // pack top bar to root box
    gtk_box_pack_start(GTK_BOX(this->root_box), this->top_bar, FALSE, FALSE, 0);


    this->login_form = LoginWindow::create_login_form();
    gtk_box_pack_start(GTK_BOX(this->root_box), this->login_form, FALSE, FALSE, 0);

//    this->signup_form = LoginWindow::create_signup_form();
//    gtk_box_pack_start(GTK_BOX(this->root_box), this->signup_form, FALSE, FALSE, 0);

    // error box
    this->label_error = gtk_label_new("");
    gtk_box_pack_end(GTK_BOX(this->root_box), this->label_error, FALSE, FALSE, 0);


    gtk_container_add(GTK_CONTAINER(this->window), this->root_box);
}

GtkWidget * LoginWindow::widget() {
    return this->window;
}

GtkWidget * LoginWindow::create_field(const char * icon_name, const char * icon_id, const char * input_ph, const char * input_id) {
    GtkWidget * box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    GtkWidget * icon = gtk_image_new_from_icon_name(icon_name, GTK_ICON_SIZE_BUTTON);
    gtk_widget_set_name(icon, icon_id);
    GtkWidget * input = gtk_entry_new();
    gtk_widget_set_name(input, input_id);

    gtk_entry_set_placeholder_text(GTK_ENTRY(input), input_ph);

    gtk_widget_set_margin_start(icon, 140);
    gtk_widget_set_margin_end(icon, 40);

    gtk_box_pack_start(GTK_BOX(box), icon, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), input, FALSE, FALSE, 0);

    return box;
}

GtkWidget * LoginWindow::create_login_form() {
    GtkWidget * root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    // field username
    GtkWidget * field_username = LoginWindow::create_field("applications-development", "icon_login_username", "username", "entry_login_username");
    gtk_widget_set_name(field_username, "field_username");
    // field password
    GtkWidget * field_password = LoginWindow::create_field("go-bottom", "icon_login_pw", "password", "entry_login_pw");
    gtk_widget_set_name(field_password, "field_password");

    gtk_box_pack_start(GTK_BOX(root), field_username, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(root), field_password, FALSE, FALSE, 0);

    // remember me
    GtkWidget * check = gtk_check_button_new_with_label("remember me");
    gtk_widget_set_name(check, "login_check");
    gtk_box_pack_start(GTK_BOX(root), check, FALSE, FALSE, 0);

    // button
    GtkWidget * confirm_button = gtk_button_new_with_label("666");
    gtk_widget_set_name(confirm_button, "button_login_confirm");
    g_signal_connect(confirm_button, "clicked", G_CALLBACK(LoginWindow::btn_login_clicked), this);

    gtk_box_pack_start(GTK_BOX(root), confirm_button, FALSE, FALSE, 0);

    return root;
}

GtkWidget * LoginWindow::create_signup_form() {
    GtkWidget * root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget * field_username = LoginWindow::create_field(
            "applications-development",
            "icon_signup_username",
            "username",
            "entry_signup_username");
    gtk_widget_set_name(field_username, "field_username");
    GtkWidget * field_password = LoginWindow::create_field(
            "go-bottom",
            "icon_signup_pw",
            "password",
            "entry_signup_pw");
    gtk_widget_set_name(field_password, "field_password");
    GtkWidget * field_cfm_password = LoginWindow::create_field(
            "go-up",
            "icon_signup_cfm_pw",
            "confirm password",
            "entry_signup_cfm_pw"
            );
    gtk_widget_set_name(field_cfm_password, "field_cfm_password");

    gtk_box_pack_start(GTK_BOX(root), field_username, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(root), field_password, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(root), field_cfm_password, FALSE, FALSE, 0);

    GtkWidget * confirm_button = gtk_button_new_with_label("666");
    gtk_widget_set_name(confirm_button, "button_signup_confirm");
    g_signal_connect(confirm_button, "clicked", G_CALLBACK(LoginWindow::btn_signup_clicked), this);

    gtk_box_pack_start(GTK_BOX(root), confirm_button, FALSE, FALSE, 0);

    return root;
}

void LoginWindow::btn_signup_switch_clicked(GtkWidget * widget, gpointer data) {
    auto window = (LoginWindow *)data;

    if (window->signup_form == nullptr) {
        window->signup_form = window->create_signup_form();
        gtk_box_pack_start(GTK_BOX(window->root_box), window->signup_form, FALSE, FALSE, 0);
    }

    gtk_widget_hide(window->login_form);
    gtk_widget_show_all(window->signup_form);
}

void LoginWindow::btn_login_switch_clicked(GtkWidget *widget, gpointer data) {
    auto window = (LoginWindow *)data;

    if (window->signup_form) {
        gtk_widget_hide(window->signup_form);
    }

    gtk_widget_show_all(window->login_form);
}

void LoginWindow::btn_login_clicked(GtkWidget *widget, gpointer data) {
    auto window = (LoginWindow *)data;

//    GtkWidget * entry_username = Utils::find_child(form, "entry_login_username");

    std::string username = gtk_entry_get_text(GTK_ENTRY(Utils::find_child(window->widget(), "entry_login_username")));
    std::string password = gtk_entry_get_text(GTK_ENTRY(Utils::find_child(window->widget(), "entry_login_pw")));

    std::string info;
    bool ret = Client::login(username, password, info);

    if (ret) {
        printf("successful\n");
        window->func();
    } else {
        window->set_error_info("username or password not correct");
        return;
    }
}

void LoginWindow::btn_signup_clicked(GtkWidget *widget, gpointer data) {
    auto window = (LoginWindow *)data;

    std::string username = gtk_entry_get_text(GTK_ENTRY(Utils::find_child(window->widget(), "entry_signup_username")));
    std::string password = gtk_entry_get_text(GTK_ENTRY(Utils::find_child(window->widget(), "entry_signup_pw")));
    std::string cfm_password = gtk_entry_get_text(GTK_ENTRY(Utils::find_child(window->widget(), "entry_signup_cfm_pw")));

//    std::cout << username << password << cfm_password << std::endl;

    if (password != cfm_password) {
        window->set_error_info("密码与确认密码不一致!");
        return;
    }

    std::string info;
    bool ret = Client::signup(username, password, cfm_password, info);

    if (ret) {
        printf("signup successful\n");
        window->func();
    } else {
        window->set_error_info(info.c_str());
        return;
    }
}

void LoginWindow::set_error_info(const char *info) {
    gtk_widget_set_name(this->label_error, "label_error");
    gtk_label_set_text(GTK_LABEL(this->label_error), info);
}

LoginWindow * LoginWindow::create() {
    return new LoginWindow();
}

void LoginWindow::set_login_success_callback(std::function<void()> && func) {
    this->func = func;
}