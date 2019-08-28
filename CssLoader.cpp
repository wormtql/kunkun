//
// Created by iatow on 19-8-27.
//

#include "INCLUDE.h"

using std::ifstream;
using std::string;

CssLoader * CssLoader::getIns() {
    static CssLoader ins;

    return &ins;
}

string CssLoader::load_css_file(const char *filename) {
    ifstream file(filename);
    string ans = "";

    if (!file.is_open()) {
        printf("error opening css file: %s\n", filename);
        return "";
    }

    string line;
    while (getline(file, line)) {
        ans += line;
    }

    return ans;
}

void CssLoader::apply_css_to_widget(GtkWidget *widget, const char *filename) {
    string css = load_css_file(filename);

//    std::cout << css;
//    fflush(stdout);

    GtkCssProvider * provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css.c_str(), -1, NULL);

//    GdkDisplay * display = gdk_display_get_default();
//    GdkScreen * screen = gdk_display_get_default_screen(display);
    GtkStyleContext * context = gtk_widget_get_style_context(widget);

//    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    gtk_style_context_add_provider(context, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);


    g_object_unref(provider);
}

void CssLoader::add_css_global(const char *filename) {
    string css = load_css_file(filename);

    GtkCssProvider * provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css.c_str(), -1, NULL);

    GdkDisplay * display = gdk_display_get_default();
    GdkScreen * screen = gdk_display_get_default_screen(display);

    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

//    g_object_unref(provider);
    this->global_css_providers.push_back(provider);
}

void CssLoader::remove_all_global_provider() {
    this->global_css_providers.clear();
}