//
// Created by iatow on 19-8-30.
//

#include "INCLUDE.h"

#include "ConsolePanel.h"

ConsolePanel::ConsolePanel() {
    root = gtk_scrolled_window_new(nullptr, nullptr);

    text_view = gtk_text_view_new();
    text_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    
}

ConsolePanel* ConsolePanel::create() {
    return new ConsolePanel;
}


GtkWidget* ConsolePanel::widget() {
    return root;
}
