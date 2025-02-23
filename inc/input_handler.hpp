#pragma once

#include "editor.hpp"

#include <SDL3/SDL.h>



class
InputHandler
{
public:
    /// Handles scancode inputs
    /// \param EditorData _EditorData struct
    /// \param scancode user input as scancode
    static SDL_AppResult Handle(_EditorData *EditorData, SDL_Scancode scancode);

    InputHandler() = delete;
private:

    /// Handles backsppace input
    /// \param EditorData _EditorData struct
    /// \param is_ctrl_pressed an indicator of whether the LCtrl key is pressed
    /// \return will return true on "expect to be rendered",
    // and returns false if nothing will be changed
    static bool Handle_Backspace(_EditorData *EditorData, bool is_ctrl_pressed);

    /// Handles the LCtrl binding of backspace
    /// \param EditorData _EditorData struct
    static void Handle_CTRL_Backspace(_EditorData *EditorData);
};