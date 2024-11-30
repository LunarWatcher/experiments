#include "allegro5/allegro_primitives.h"
#include <iostream>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/display.h>

#include <Agui/Agui.hpp>
#include <Agui/Backends/Allegro5/Allegro5.hpp>
#include <Agui/Widgets/Button/Button.hpp>
#include <Agui/FlowLayout.hpp>

int main() {
    constexpr auto CJK_FONT = "/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc";
    constexpr auto DEFAULT_FONT = "/usr/share/fonts/truetype/ubuntu/Ubuntu[wdth,wght].ttf";

    if(!al_init()) {
        std::cout << "couldn't initialize allegro" << std::endl;
        return 1;
    }

    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
    al_install_keyboard();


    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);


    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();

    ALLEGRO_DISPLAY* disp = al_create_display(640, 480);
    al_set_window_title(disp, "Choo choo");

    ALLEGRO_FONT* font = al_load_ttf_font(CJK_FONT, 36, 0);


    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    agui::Gui* gui;
    agui::Allegro5Input* inputHandler;
    agui::Allegro5Graphics* graphicsHandler;

    // Garbage practice, but arsed doing this properly. It'll be fine
    // (No, these are not `delete`d anywhere. It'll be deleted when the program shuts down. 
    // This needs to be handled properly in a real game)
    agui::Image::setImageLoader(new agui::Allegro5ImageLoader);
    agui::Font::setFontLoader(new agui::Allegro5FontLoader);
    inputHandler = new agui::Allegro5Input();
    graphicsHandler = new agui::Allegro5Graphics();
    gui = new agui::Gui();

    gui->setInput(inputHandler);
    gui->setGraphics(graphicsHandler);
    auto uiFont = agui::Font::load(CJK_FONT, 16);
    agui::Widget::setGlobalFont(uiFont);

    agui::FlowLayout flow;
    gui->add(&flow);
    flow.setMargins(200, 0, 0, 0);

    agui::Button btn;
    flow.add(&btn);
    btn.setSize(400, 30);
    btn.setText("Push me uwu æøå ㄴㅇㄴㅁㄹㅇㄴ엉");

    // Bit annoying that there's now two ways to init the font, but looks like there's no way around it.
    // With imgui in the mix for debugging stuff, that's potentially three separate ways to load fonts (not sure how
    // imgui works, future me problem)

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

    al_start_timer(timer);
    while(1) {
        al_wait_for_event(queue, &event);

        switch(event.type) {
            case ALLEGRO_EVENT_TIMER:
                redraw = true;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        inputHandler->processEvent(event);

        if(done)
            break;

        if(redraw && al_is_event_queue_empty(queue)) {

            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Hello world! 여오");

            if (gui) {
                gui->logic();
                gui->render();
            }

            al_flip_display();


            redraw = false;
        }
    }

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);


    return 0;
}
