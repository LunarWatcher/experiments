#include "gtkmm/application.h"

#include <gtkmm.h>

class WindowImpl : public Gtk::Window {
private:
    Gtk::Frame f;
    Gtk::Grid b;
    Gtk::Button btn{"owo"};
    Gtk::Button btn2{"owo"};
public:
    WindowImpl() {
        set_title("*pounces* rawr x3");
        set_default_size(720, 480);

        f.set_margin(10);
        f.set_label("danger zone");

        set_child(f);
        f.set_child(b);

        b.attach(btn, 0, 0);
        b.attach(btn2, 1, 0);
        b.attach_next_to(btn2, btn, Gtk::PositionType::RIGHT, 2, 0);
    }
};

int main(int argc, char** argv) {
    auto app = Gtk::Application::create(
        "lunarwatcher.test"
    );

    return app->make_window_and_run<WindowImpl>(argc, argv);
}
