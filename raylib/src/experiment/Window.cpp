#include "Window.hpp"

#include <iostream>

#include "raylib.h"

namespace ex {

void Window::loop() {
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(800, 600, "Raylib experiment");
    SetTargetFPS(60);

    auto font = LoadFontEx(
        "/usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf",
        20,
        nullptr,
        // There aren't even this many characters in unicode, but whatever
        2'000'000
    );

    while (!WindowShouldClose()) {
        BeginDrawing();
        
        // Setup the back buffer for drawing (clear color and depth buffers)
        ClearBackground(BLACK);

        DrawTextEx(font, "Hello Raylib", {200, 200}, 20, 1, WHITE);
        DrawTextEx(font, "여우입니다", {200, 220}, 20, 1, WHITE);
        DrawTextEx(font, "æøå", {200, 240}, 20, 1, WHITE);
        
        DrawFPS(0, 0);
        EndDrawing();
    }

    UnloadFont(font);

    CloseWindow();
}

}
