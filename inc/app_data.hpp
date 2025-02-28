#pragma once

#include "editor.hpp"


struct AppData {
    struct SDL_Renderer *renderer = nullptr;
    struct SDL_Window *window = nullptr;
    struct TTF_Font *font = nullptr;

    bool focused = false;
    bool changed = false;
    bool verbose = false;

    EditorData editor_data;

    AppData(EditorData *_editor_data) :
        editor_data(_editor_data)
        {};
};