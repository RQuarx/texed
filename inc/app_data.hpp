#pragma once

#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL.h>

#include "editor.hpp"


struct AppData {
    SDL_Renderer *renderer;
    SDL_Window *window;
    TTF_Font *font;

    bool focused;
    bool changed;
    bool verbose;

    _EditorData EditorData;

    AppData() :
    renderer(NULL),
    window(NULL),
    font(NULL),
    focused(false),
    changed(false),
    verbose(false),
    EditorData({""}, "") {};
};