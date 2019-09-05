//
// Created by iatow on 19-8-27.
//

#include "INCLUDE.h"

LoginWindow::LoginWindow() {
    // window
    this->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_decorated(GTK_WINDOW(this->window), FALSE);
    gtk_widget_set_name(this->window, "login_window");
    gtk_window_set_default_size(GTK_WINDOW(this->window), 500, 800);

    // top bar
    this->top_bar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    gtk_widget_set_name(this->top_bar, "top_bar");

//    gtk_widget_set_events(this->top_bar, GDK_BUTTON_PRESS_MASK);
    gtk_widget_set_events(this->window, GDK_BUTTON_PRESS_MASK | GDK_BUTTON1_MOTION_MASK);

//    g_signal_connect(this->top_bar, "button_press_event", G_CALLBACK(LoginWindow::button_press_event), this);
    g_signal_connect(this->window, "button_press_event", G_CALLBACK(LoginWindow::button_press_event), this);
//    g_signal_connect(this->top_bar, "button_release_event", G_CALLBACK(LoginWindow::button_release_event), this);
    g_signal_connect(this->window, "button_release_event", G_CALLBACK(LoginWindow::button_release_event), this);
//    g_signal_connect(this->top_bar, "motion_notify_event", G_CALLBACK(LoginWindow::motion_notify_event), this);
    g_signal_connect(this->window, "motion_notify_event", G_CALLBACK(LoginWindow::motion_notify_event), this);


    // image
    GtkWidget * image = Utils::create_image_from_file_at_size("../assets/basketball.jpg", -1);
    gtk_widget_set_name(image, "image_header");



    // close button
//    this->close_button = gtk_button_new_with_label("x");
    this->close_button = Utils::create_button_with_icon("../assets/close.png", 12);
    gtk_widget_set_name(this->close_button, "close_button");
    g_signal_connect_swapped(this->close_button, "clicked", G_CALLBACK(gtk_widget_destroy), this->window);

    // login switch button
//    this->button_login_switch = gtk_button_new_from_icon_name("user-home", GTK_ICON_SIZE_BUTTON);
    this->button_login_switch = Utils::create_button_with_icon("../assets/login.png", 20);
    gtk_widget_set_name(this->button_login_switch, "button_login_switch");
    g_signal_connect(this->button_login_switch, "clicked", G_CALLBACK(LoginWindow::btn_login_switch_clicked), this);
    gtk_box_pack_start(GTK_BOX(this->top_bar), this->button_login_switch, FALSE, FALSE, 0);


    // signup switch button
//    this->button_signup_switch = gtk_button_new_from_icon_name("user-idle", GTK_ICON_SIZE_BUTTON);
    this->button_signup_switch = Utils::create_button_with_icon("../assets/signup.png", 20);
    gtk_widget_set_name(this->button_signup_switch, "button_signup_switch");
    g_signal_connect(this->button_signup_switch, "clicked", G_CALLBACK(LoginWindow::btn_signup_switch_clicked), this);
    gtk_box_pack_start(GTK_BOX(this->top_bar), this->button_signup_switch, FALSE, FALSE, 0);

    // pack close button to top bar
    gtk_box_pack_end(GTK_BOX(this->top_bar), this->close_button, FALSE, FALSE, 0);

    // root box
    this->root_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    // pack top bar to root box
    gtk_box_pack_start(GTK_BOX(this->root_box), this->top_bar, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(this->root_box), image, FALSE, FALSE, 0);


    this->login_form = LoginWindow::create_login_form();
    gtk_box_pack_start(GTK_BOX(this->root_box), this->login_form, FALSE, FALSE, 0);

//    this->signup_form = LoginWindow::create_signup_form();
//    gtk_box_pack_start(GTK_BOX(this->root_box), this->signup_form, FALSE, FALSE, 0);

    // error box
//    this->label_error = gtk_label_new("");
//    gtk_box_pack_end(GTK_BOX(this->root_box), this->label_error, FALSE, FALSE, 0);


    gtk_container_add(GTK_CONTAINER(this->window), this->root_box);
}

GtkWidget * LoginWindow::widget() {
    return this->window;
}

GtkWidget * LoginWindow::create_field(const char * icon_name, const char * icon_id, const char * input_ph, const char * input_id) {
    GtkWidget * box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

//    GtkWidget * icon = gtk_image_new_from_icon_name(icon_name, GTK_ICON_SIZE_BUTTON);
    GtkWidget * icon = Utils::create_image_from_file_at_size(icon_name, 24);
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
    GtkWidget * field_username = LoginWindow::create_field("../assets/user.png", "icon_login_username", "username", "entry_login_username");
    gtk_widget_set_name(field_username, "field_username");
    // field password
    GtkWidget * field_password = LoginWindow::create_field("../assets/lock.png", "icon_login_pw", "password", "entry_login_pw");
    GtkWidget * entry = Utils::find_child(field_password, "entry_login_pw");
    gtk_entry_set_visibility(GTK_ENTRY(entry), FALSE);

    gtk_widget_set_name(field_password, "field_password");

    gtk_box_pack_start(GTK_BOX(root), field_username, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(root), field_password, FALSE, FALSE, 0);

    // remember me
    GtkWidget * check = gtk_check_button_new_with_label("remember me");
    gtk_widget_set_name(check, "login_check");
    gtk_box_pack_start(GTK_BOX(root), check, FALSE, FALSE, 0);

    // button
//    GtkWidget * confirm_button = gtk_button_new_with_label("666");
    GtkWidget * confirm_button = Utils::create_button_with_icon("../assets/key-white.png", 24);
    gtk_widget_set_name(confirm_button, "button_login_confirm");
    g_signal_connect(confirm_button, "clicked", G_CALLBACK(LoginWindow::btn_login_clicked), this);

    gtk_box_pack_start(GTK_BOX(root), confirm_button, FALSE, FALSE, 0);

    return root;
}

GtkWidget * LoginWindow::create_signup_form() {
    GtkWidget * root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    GtkWidget * field_username = LoginWindow::create_field(
            "../assets/user.png",
            "icon_signup_username",
            "username",
            "entry_signup_username");
    gtk_widget_set_name(field_username, "field_username");
    GtkWidget * field_password = LoginWindow::create_field(
            "../assets/lock.png",
            "icon_signup_pw",
            "password",
            "entry_signup_pw");
    GtkWidget * entry = Utils::find_child(field_password, "entry_signup_pw");
    gtk_entry_set_visibility(GTK_ENTRY(entry), FALSE);
    gtk_widget_set_name(field_password, "field_password");
    GtkWidget * field_cfm_password = LoginWindow::create_field(
            "../assets/lock.png",
            "icon_signup_cfm_pw",
            "confirm password",
            "entry_signup_cfm_pw"
            );
    entry = Utils::find_child(field_cfm_password, "entry_signup_cfm_pw");
    gtk_entry_set_visibility(GTK_ENTRY(entry), FALSE);
    gtk_widget_set_name(field_cfm_password, "field_cfm_password");

    gtk_box_pack_start(GTK_BOX(root), field_username, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(root), field_password, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(root), field_cfm_password, FALSE, FALSE, 0);

//    GtkWidget * confirm_button = gtk_button_new_with_label("666");
    GtkWidget * confirm_button = Utils::create_button_with_icon("../assets/key-white.png", 24);
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

    bool succ = false;
    std::string msg;

    if (!Utils::check_valid_string(username)) {
        window->set_error_info("字段必须为数字、大小写字母、下划线");
        return;
    }

    if (!Utils::check_valid_string(password)) {
        window->set_error_info("字段必须为数字、大小写字母、下划线");
        return;
    }

    if (username.empty()) {
        window->set_error_info("用户名不能为空");
        return;
    }

    if (password.empty()) {
        window->set_error_info("密码不能为空");
        return;
    }

    json ret = ClientUtils::login_blocked(username, password);

    if (ret["debug"]) {
        succ = true;
    } else {
        if (ret["status"]) {
            succ = true;
        } else {
            succ = false;
            msg = ret["msg"];
        }
    }


    if (succ) {
        printf("login successful\n");
        DataHub::getIns()->username = username;
        window->func();
    } else {
        window->set_error_info(msg.c_str());
        return;
    }
}

void LoginWindow::btn_signup_clicked(GtkWidget *widget, gpointer data) {
    auto window = (LoginWindow *)data;

    std::string username = gtk_entry_get_text(GTK_ENTRY(Utils::find_child(window->widget(), "entry_signup_username")));
    std::string password = gtk_entry_get_text(GTK_ENTRY(Utils::find_child(window->widget(), "entry_signup_pw")));
    std::string cfm_password = gtk_entry_get_text(GTK_ENTRY(Utils::find_child(window->widget(), "entry_signup_cfm_pw")));

    if (!Utils::check_valid_string(username)) {
        window->set_error_info("字段必须为数字、大小写字母、下划线");
        return;
    }

    if (!Utils::check_valid_string(password)) {
        window->set_error_info("字段必须为数字、大小写字母、下划线");
        return;
    }

    if (!Utils::check_valid_string(cfm_password)) {
        window->set_error_info("字段必须为数字、大小写字母、下划线");
        return;
    }

    if (username.empty()) {
        window->set_error_info("用户名不能为空");
        return;
    }

    if (password.empty() || cfm_password.empty()) {
        window->set_error_info("密码不能为空");
        return;
    }

    if (password != cfm_password) {
        window->set_error_info("密码与确认密码不一致!");
        return;
    }

    bool succ = false;
    std::string info;

    json ret = ClientUtils::signup_blocked(username, password);


    if (ret["debug"]) {
        succ = false;
        info = "debug";
    } else {
        succ = ret["status"];
        info = ret["msg"];
    }

    if (succ) {
        printf("signup successful\n");
        DataHub::getIns()->username = username;
        window->func();
    } else {
        window->set_error_info(info.c_str());
        return;
    }
}

void LoginWindow::set_error_info(const char *info) {
//    gtk_widget_set_name(this->label_error, "label_error");
//    gtk_label_set_text(GTK_LABEL(this->label_error), info);
    GtkWidget * lbl = gtk_label_new(info);
    gtk_widget_set_name(lbl, "label_error");

    GtkWidget * prev = Utils::find_child(this->root_box, "label_error");
    if (prev) {
        gtk_widget_destroy(prev);
    } else {

    }

    gtk_widget_show_all(lbl);
    gtk_box_pack_end(GTK_BOX(this->root_box), lbl, FALSE, FALSE, 0);

    g_timeout_add(5000, [] (gpointer data) -> gboolean {

        auto widget = (GtkWidget *)data;

        if (GTK_IS_WIDGET(widget)) {
            gtk_widget_destroy(widget);
        }

        return false;

    }, lbl);
}

LoginWindow * LoginWindow::create() {
    return new LoginWindow();
}

void LoginWindow::set_login_success_callback(std::function<void()> && func) {
    this->func = func;
}

gint LoginWindow::button_press_event(GtkWidget *widget, GdkEventButton *event, gpointer data) {
//    printf("worm\n");

    auto window = (LoginWindow *)data;

    window->x = (int)event->x;
    window->y = (int)event->y;
    window->drag = true;

    return TRUE;
}

gint LoginWindow::button_release_event(GtkWidget *widget, GdkEventButton *event, gpointer data) {

//    printf("release\n");

    auto window = (LoginWindow *)data;

    window->drag = false;

    return TRUE;
}

gint LoginWindow::motion_notify_event(GtkWidget *widget, GdkEventButton *event, gpointer data) {

//    printf("motion\n");

    auto window = (LoginWindow *)data;
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