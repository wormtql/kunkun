#include "INCLUDE.h"

#include "LoginWindow.h"

using std::string;
using std::ofstream;
using std::cout;
using std::endl;

static void activate(GtkApplication * app, gpointer data) {
    CssLoader::getIns()->add_css_global("../css/login_window_style.css");
    CssLoader::getIns()->add_css_global("../css/main_window_style.css");
    CssLoader::getIns()->add_css_global("../css/chat_panel_style.css");
    CssLoader::getIns()->add_css_global("../css/user_panel_style.css");
    CssLoader::getIns()->add_css_global("../css/add_friend_panel_style.css");
    CssLoader::getIns()->add_css_global("../css/console_panel_style.css");

    LoginWindow * loginWindow = LoginWindow::create();
    MainWindow * mainWindow = nullptr;

    loginWindow->set_login_success_callback([=, &mainWindow] () {
        MainWindow * window = MainWindow::create();
        mainWindow = window;

        gtk_application_add_window(app, GTK_WINDOW(window->widget()));

        gtk_widget_show_all(window->widget());
        gtk_widget_destroy(loginWindow->widget());
    });


    gtk_application_add_window(app, GTK_WINDOW(loginWindow->widget()));
//    gtk_application_add_window(app, GTK_WINDOW(mainWindow->widget()));

    gtk_widget_show_all(loginWindow->widget());
}

int main(int argc, char * argv[]) {

    // create file directory
    if (access("files", 0) != 0) {
        mkdir("files", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }

    // init client socket
    Client::getIns()->initialize_net();

    // launch thread for listening message
    Thread::init();

    GtkApplication * app;
    int status;

    app = gtk_application_new(nullptr, G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);



    // kill thread
    Thread::kill();

    return status;
}