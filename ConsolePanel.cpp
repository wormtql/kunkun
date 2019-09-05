//
// Created by iatow on 19-8-30.
//

#include "INCLUDE.h"

#include "ConsolePanel.h"

ConsolePanel::ConsolePanel() {
    root = gtk_scrolled_window_new(nullptr, nullptr);
    gtk_widget_set_name(root, "console_panel");

    text_view = gtk_text_view_new();
    gtk_widget_set_name(text_view, "text_view");
    g_signal_connect(text_view, "key-press-event", G_CALLBACK(ConsolePanel::on_key_pressed), this);
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    gtk_text_buffer_set_text(text_buffer, ">>> ", 4);

    gtk_container_add(GTK_CONTAINER(root), text_view);

//    GtkTextIter * iter = nullptr;
//    gtk_text_view_get_iter_at_location(GTK_TEXT_VIEW(text_view), iter, 0, 0);

//    char temp[] = "<p style=\"color:red;\">ajdad</p>";

//    gtk_text_buffer_insert_markup(text_buffer, iter, temp, sizeof(temp));
}

ConsolePanel* ConsolePanel::create() {
    return new ConsolePanel;
}


GtkWidget* ConsolePanel::widget() {
    return root;
}

gboolean ConsolePanel::on_key_pressed(GtkWidget *widget, GdkEventKey *event, gpointer data) {
    auto window = (ConsolePanel *)data;

    if (strcmp(gdk_keyval_name(event->keyval), "Return") == 0) {
        int line_count = gtk_text_buffer_get_line_count(window->text_buffer);

        GtkTextIter start;
        GtkTextIter end;
        gtk_text_buffer_get_iter_at_line(window->text_buffer, &start, line_count - 1);
        gtk_text_buffer_get_iter_at_line(window->text_buffer, &end, 1000000);

        char * text = gtk_text_iter_get_text(&start, &end);

        std::string command;
        int len = (int)strlen(text);
        for (int i = 0; i < len; i++) {
            if (text[i] != '>') {
                command.push_back(text[i]);
                if (command == " ") {
                    command = "";
                }
            }
        }

        window->my_print("\n");
        window->process_command(command);
        window->my_print(">>> ");

        return true;
    }

    return false;
//    return true;
}

void ConsolePanel::my_print(const std::string &str) {
    GtkTextIter end;
    gtk_text_buffer_get_iter_at_line(text_buffer, &end, 100000);
    gtk_text_buffer_insert(text_buffer, &end, str.c_str(), sizeof(char) * str.size());
}

void ConsolePanel::process_command(const std::string &cmd) {
    std::vector<std::string> argv;
    argv.emplace_back("");
    int argc = 1;

    for (char c : cmd) {
        if (c == ' ')
        {
            argc++;
            argv.emplace_back("");
        }
        else
        {
            argv[argc - 1].push_back(c);
        }
    }


    bool too_few_arg_flag = false;

    // todo
    // parse argument here
    if (argv[0] == "del")
    {

    }
    else if (argv[0] == "set")
    {

    }
    else if (argv[0] == "get")
    {

    }
    else if (argv[0] == "set")
    {

    }
    else if (argv[0] == "ls")
    {
        if (argv.size() <= 1)
            too_few_arg_flag = true;
        else {
            if (argv[1] == "request") {
                if (argv.size() <= 2) {
                    too_few_arg_flag = true;
                } else {
                    if (argv[2] == "f-req") {
                        json ret = ClientUtils::list_friend_request(DataHub::getIns()->username);
                        my_print(ret.dump(4));
                        my_print("\n");
                    } else if (argv[2] == "g-inv") {
                        json ret = ClientUtils::list_group_invitation(DataHub::getIns()->username);
                        my_print(ret.dump(4));
                        my_print("\n");
                    } else if (argv[2] == "g-req") {
                        json ret = ClientUtils::list_group_request(DataHub::getIns()->username);
                        my_print(ret.dump(4));
                        my_print("\n");
                    }
                }
            } else if (argv[1] == "friend") {

            } else if (argv[1] == "group") {

            }
        }
    }
    else if (argv[0] == "msg")
    {

    }
    else if (argv[0] == "req")
    {

    }
    else if (argv[0] == "res")
    {
        if (argv.size() <= 1) {
            too_few_arg_flag = true;
        }
        else {
            bool ans = false;
            if (argv[1] == "true") {
                ans = true;
            } else if (argv[1] == "false") {
                ans = false;
            } else {
                my_print("res [true | false] options\n");
                return;
            }

            if (argv.size() < 3) {
                too_few_arg_flag = true;
            } else {
                if (argv[2] == "f-req") {

                    if (argv.size() < 4) {
                        too_few_arg_flag = true;
                    } else {
                        ClientUtils::send_add_friend_result(argv[3], DataHub::getIns()->username, ans);
                        my_print("sent\n");
                    }

                } else if (argv[2] == "g-req") {

                    if (argv.size() < 4) {
                        too_few_arg_flag = true;
                    } else {
                        ClientUtils::send_join_group_result(argv[3], argv[4], ans);
                        my_print("sent\n");
                    }

                } else if (argv[2] == "g-inv") {

                    if (argv.size() < 5) {
                        too_few_arg_flag = true;
                    } else {
                        ClientUtils::send_invite_to_group_result(DataHub::getIns()->username, argv[3], ans);
                        my_print("sent\n");
                    }

                }
            }
        }
    }
    else if (argv[0] == "clear")
    {
        gtk_text_buffer_set_text(text_buffer, "", 0);
    }
    else if (argv[0] == "create") {
        if (argv.size() < 2) {
            too_few_arg_flag = true;
        } else {
            if (argv[1] == "group") {
                if (argv.size() < 3) {
                    too_few_arg_flag = true;
                } else {
                    ClientUtils::create_group(DataHub::getIns()->username, argv[2]);
                }
            }
        }
    }
    else if (argv[0] == "help") {
        my_print(R"(del
    group
        [group_id]
        [group_name]
    friend
        [username]

set
    username
        [new_value]
    sign
        [new_value]
    gender
        [new_value]

get
    username
    sign
    gender

ls
    group
        group_id
        group_name
    friend
        friend_name
    request
        f-req
        g-inv
        g-req

msg
    group
        group_id
            msg
    friend
        friend_id
            msg

req
    add
        friend
            username
        group
            group_id

res
    [true | false]
        f-req
            username
        g-inv
            group_id
        g-req
            username
                group_id

create
    group
        name


clear)");
        my_print("\n");
    }
    else
    {
        my_print("unrecognized command " + argv[0] + "\n");
    }
}