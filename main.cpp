#include "INCLUDE.h"

#include "LoginWindow.h"

static void activate(GtkApplication * app, gpointer data) {
    CssLoader::getIns()->add_css_global("../css/login_window_style.css");

    LoginWindow * loginWindow = new LoginWindow();
    GtkWidget * window = loginWindow->widget();

    gtk_application_add_window(app, GTK_WINDOW(window));

    gtk_widget_show_all(window);
}

int main(int argc, char * argv[]) {
    GtkApplication * app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}