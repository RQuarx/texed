#pragma once
#include "editor.hpp"

#define IS_CTRL_PRESSED (SDL_GetModState() | SDL_KMOD_LCTRL)


class
Movement
{
public:
    static bool Move_Cursor(SDL_Scancode scancode, EditorData *editor_data, bool is_ctrl_pressed);

    static bool Move_Cursor_Right(EditorData *editor_data, bool is_ctrl_pressed);
    static bool Move_Cursor_Down(EditorData *editor_data, bool is_ctrl_pressed);
    static bool Move_Cursor_Left(EditorData *editor_data, bool is_ctrl_pressed);
    static bool Move_Cursor_Up(EditorData *editor_data, bool is_ctrl_pressed);

    Movement() = delete;
};