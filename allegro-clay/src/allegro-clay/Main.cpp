#include "allegro5/allegro_primitives.h"
#include <iostream>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/display.h>

#include "allegro5/events.h"
#include "allegro5/timer.h"

#include <clay.h>

static inline Clay_Dimensions MeasureText(Clay_StringSlice text, Clay_TextElementConfig *config, uintptr_t userData) {
    return (Clay_Dimensions) {
            .width = (float) text.length * config->fontSize,
            .height = 32
    };
}
void HandleClayErrors(Clay_ErrorData errorData) {
    // See the Clay_ErrorData struct for more information
    std::cerr << errorData.errorText.chars << std::endl;
}

int main() {
    // General allegro boilerplate {{{
    constexpr auto FONT = "./assets/dejavu.ttf";

    if(!al_init()) {
        std::cout << "couldn't initialize allegro" << std::endl;
        return 1;
    }

    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();

    al_install_keyboard();
    al_install_mouse();

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();

    ALLEGRO_DISPLAY* disp = al_create_display(640, 480);
    al_set_window_title(disp, "Choo choo");

    ALLEGRO_FONT* font = al_load_ttf_font(FONT, 36, 0);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_display_event_source(disp));

    bool done = false;
    bool redraw = true;
    ALLEGRO_EVENT event;
    // }}}

    uint64_t totalMemorySize = Clay_MinMemorySize();
    Clay_Arena arena = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, malloc(totalMemorySize));

    // Note: screenWidth and screenHeight will need to come from your environment, Clay doesn't handle window related tasks
    Clay_Initialize(arena, (Clay_Dimensions) { 640, 480 }, (Clay_ErrorHandler) { HandleClayErrors, nullptr });

    al_start_timer(timer);
    while(true) {
        al_wait_for_event(queue, &event);

        switch(event.type) { // NOLINT
        case ALLEGRO_EVENT_TIMER:
            redraw = true;
            break;

        case ALLEGRO_EVENT_DISPLAY_CLOSE:
            done = true;
            break;

        case ALLEGRO_EVENT_MOUSE_AXES:
        case ALLEGRO_EVENT_MOUSE_WARPED:
        case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
        case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
            Clay_SetPointerState((Clay_Vector2) { (float) event.mouse.x, (float) event.mouse.y }, event.mouse.button != 0);
            Clay_UpdateScrollContainers(true, (Clay_Vector2) { (float) event.mouse.dx, (float) event.mouse.dy }, 1.0 / 60.0);
            break;
        }

        if(done) {
            break;
        }

        if(redraw && al_is_event_queue_empty(queue)) {

            al_clear_to_color(al_map_rgb(0, 0, 0));

            Clay_BeginLayout();

            // Draw two dummy nested rects
            CLAY({ .id = CLAY_ID("MainContent"), .layout = { .padding = CLAY_PADDING_ALL(16), .sizing = { .width = CLAY_SIZING_GROW(0), .height = CLAY_SIZING_GROW(0) } }, .backgroundColor = Clay_Color { 255, 0, 225, 255 }, }) {
                CLAY( {.layout = { .sizing = {.width = CLAY_SIZING_GROW(0), .height = 200.f } }, .backgroundColor = Clay_Color { 0, 100, 255, 255 }} ) {
                }
            }


            Clay_RenderCommandArray renderCommands = Clay_EndLayout();

            for (int i = 0; i < renderCommands.length; i++) {
                Clay_RenderCommand *renderCommand = &renderCommands.internalArray[i];

                switch (renderCommand->commandType) {
                    case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
                        al_draw_filled_rectangle(
                            renderCommand->boundingBox.x,
                            renderCommand->boundingBox.y,
                            renderCommand->boundingBox.x + renderCommand->boundingBox.width,
                            renderCommand->boundingBox.y + renderCommand->boundingBox.height,
                            al_map_rgba_f(
                                renderCommand->renderData.rectangle.backgroundColor.r,
                                renderCommand->renderData.rectangle.backgroundColor.g,
                                renderCommand->renderData.rectangle.backgroundColor.b,
                                renderCommand->renderData.rectangle.backgroundColor.a
                            )
                        );
                    }
                }
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
