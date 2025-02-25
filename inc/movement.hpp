#pragma once
#include "editor.hpp"

#define IS_CTRL_PRESSED (SDL_GetModState() | SDL_KMOD_LCTRL)


class
Movement
{
public:
    static bool Move_Cursor_Right(_EditorData *EditorData, bool is_ctrl_pressed);
    static bool Move_Cursor_Down(_EditorData *EditorData, bool is_ctrl_pressed);
    static bool Move_Cursor_Left(_EditorData *EditorData, bool is_ctrl_pressed);
    static bool Move_Cursor_Up(_EditorData *EditorData, bool is_ctrl_pressed);

    Movement() = delete;
};