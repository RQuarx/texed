#pragma once

#include "app_data.hpp"

#include <SDL3/SDL.h>

struct size_c {
    size_t x;
    size_t y;
};

struct VisualData {
    size_c start_pos;
    size_c current_pos;
};


class
RenderCursor
{
public:
    static bool Hollow(SDL_Color color, AppData *AppData, Offset Offset);
    static bool Beam(SDL_Color color, AppData *AppData, Offset Offset);
    static bool Line(SDL_Color color, AppData *AppData, Offset Offset);
    static bool Box(SDL_Color color, AppData *AppData, Offset Offset);

    static bool Visual(SDL_Color color, VisualData data, AppData *AppData, Offset Offset);

    RenderCursor() = delete;
private:
    static SDL_FRect Get_Cursor_Rect(AppData *AppData, SDL_Color color, float x, float y, float w, float h);
    static bool Render(AppData *AppData, SDL_Color color, float x, float y, float w, float h);
};