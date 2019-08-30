//
// Created by iatow on 19-8-30.
//

#ifndef KUNKUN_CONSOLEPANEL_H
#define KUNKUN_CONSOLEPANEL_H


class ConsolePanel {
private:

    GtkWidget * root = nullptr;

    GtkTextBuffer * text_buffer = nullptr;
    GtkWidget * text_view = nullptr;

    ConsolePanel();

    static gboolean on_key_pressed(GtkWidget * widget, GdkEventKey * event, gpointer data);

    void process_command(const std::string & cmd);

    void my_print(const std::string & str);

public:
    static ConsolePanel * create();

    GtkWidget * widget();
};


#endif //KUNKUN_CONSOLEPANEL_H
