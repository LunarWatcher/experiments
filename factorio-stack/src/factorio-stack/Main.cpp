#include "allegro5/allegro_image.h"
#include "allegro5/allegro_primitives.h"
#include <iostream>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/display.h>

#include <Agui/Agui.hpp>
#include <Agui/Backends/Allegro5/Allegro5.hpp>
#include <Agui/Widgets/Button/Button.hpp>
#include "Agui/Widgets/TextField/TextField.hpp"
#include <Agui/FlowLayout.hpp>
#include "Agui/Color.hpp"
#include "allegro5/bitmap.h"
#include "allegro5/events.h"
#include "allegro5/keycodes.h"
#include "allegro5/transformations.h"


class ALTest : public agui::ActionListener {
public:
    virtual void actionPerformed(const agui::ActionEvent &evt) {
        std::cout << "I can has been clicked" << std::endl;
    }
};

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
    al_init_image_addon();
    //Borked for some reason (config error?)
    //al_init_native_dialog_addon();
    al_install_keyboard();
    al_install_mouse();

    ALLEGRO_BITMAP* bmp = al_load_bitmap("../test.png");

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);


    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();

    ALLEGRO_DISPLAY* disp = al_create_display(640, 480);
    al_set_window_title(disp, "Choo choo");
    al_set_display_icon(disp, bmp);

    ALLEGRO_FONT* font = al_load_ttf_font(CJK_FONT, 36, 0);


    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_display_event_source(disp));

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

    ALTest evl;

    gui->setInput(inputHandler);
    gui->setGraphics(graphicsHandler);
    auto uiFont = agui::Font::load(CJK_FONT, 16);
    agui::Widget::setGlobalFont(uiFont);

    agui::FlowLayout flow;
    gui->add(&flow);
    flow.setMargins(0, 0, 0, 0);
    flow.setLocation(150, 150);

    agui::Button btn;
    flow.add(&btn);
    btn.setSize(400, 30);
    btn.setText("Push me uwu æøå ㄴㅇㄴㅁㄹㅇㄴ엉");
    btn.setBackColor(agui::Color(1.0f, 0.0f, 1.0f));
    btn.setToolTipText("Test");
    btn.addActionListener(&evl);

    agui::TextField tf;
    flow.add(&tf);
    tf.setBackColor(agui::Color(0.5f, 0.5f, 0.5f));
    tf.setFontColor(agui::Color(0.9f, 0.9f, 0.9f));
    tf.setSize(400, 30);
    tf.addActionListener(&evl);
    //Action seems to be submit or completed, while other events exist for other things
    //tf.addKeyboardListener(KeyboardListener *listener)

    // Bit annoying that there's now two ways to init the font, but looks like there's no way around it.
    // With imgui in the mix for debugging stuff, that's potentially three separate ways to load fonts (not sure how
    // imgui works, future me problem)
    
    ALLEGRO_TRANSFORM worldCam, uiCam;

    al_identity_transform(&worldCam);
    al_orthographic_transform(
        &worldCam,
        0.0f, 0.0f,
        -1.0f,
        640.0f, 480.0f, 
        1.0f
    );

    al_identity_transform(&uiCam);
    al_orthographic_transform(
        &uiCam,
        0.0f, 0.0f,
        -1.0f,
        640.0f, 480.0f, 
        1.0f
    );
    


    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;

    float x = 0, y = 0;

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
            case ALLEGRO_EVENT_DISPLAY_RESIZE:
                al_acknowledge_resize(event.display.source);
                gui->resizeToDisplay();
                break;
            case ALLEGRO_EVENT_DISPLAY_SWITCH_IN:
                gui->resizeToDisplay();
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                if (event.keyboard.keycode == ALLEGRO_KEY_A) {
                    x -= 1;
                    al_translate_transform(&worldCam, -1, 0);
                } else if (event.keyboard.keycode == ALLEGRO_KEY_D) {
                    x -= 1;
                    al_translate_transform(&worldCam, 1, 0);
                }
                break;
        }

        inputHandler->processEvent(event);

        if(done)
            break;

        if(redraw && al_is_event_queue_empty(queue)) {
            gui->logic();

            al_clear_to_color(al_map_rgb(0, 0, 0));

            al_use_projection_transform(&worldCam);
            al_draw_text(font, al_map_rgb(255, 255, 255), 0, 0, 0, "Hello world! 여오");

            al_draw_bitmap(bmp, 100, 100, 0);

            al_use_projection_transform(&uiCam);
            gui->render();

            al_flip_display();


            redraw = false;
        }
    }

    al_destroy_bitmap(bmp);

    al_destroy_font(font);
    al_destroy_display(disp);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);


    return 0;
}
