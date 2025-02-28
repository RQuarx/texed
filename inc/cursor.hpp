#pragma once

#include "app_data.hpp"


class
RenderCursor
{
public:
    static bool Hollow(SDL_Color color, AppData *app_data, Offset Offset);
    static bool Beam(SDL_Color color, AppData *app_data, Offset Offset);
    static bool Line(SDL_Color color, AppData *app_data, Offset Offset);
    static bool Box(SDL_Color color, AppData *app_data, Offset Offset);

    static bool Visual(SDL_Color color, VisualData data, SDL_Renderer *renderer, Offset Offset);

    RenderCursor() = delete;
private:
    /// Returns a cursor's FRect based on the x, y, w, and h
    /// \param renderer SDL_Renderer type
    /// \param color cursor color
    /// \param x, y, w, h cursor coordinates and size
    static SDL_FRect Get_Cursor_Rect(
        SDL_Renderer *renderer,
        SDL_Color color,
        float x,
        float y,
        float w,
        float h
    );
};