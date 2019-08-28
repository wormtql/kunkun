//
// Created by iatow on 19-8-27.
//

#ifndef HOMEWORK_CSSLOADER_H
#define HOMEWORK_CSSLOADER_H


class CssLoader {
private:
    CssLoader() = default;

    std::vector<GtkCssProvider *> global_css_providers;

public:
    static CssLoader * getIns();

    std::string load_css_file(const char * filename);

    void apply_css_to_widget(GtkWidget * widget, const char * filename);

    void add_css_global(const char * filename);

    void remove_all_global_provider();
};


#endif //HOMEWORK_CSSLOADER_H
