#pragma once

#include "app_data.hpp"

#include <SDL3/SDL.h>



class
InputHandler
{
public:
    /// Handles scancode inputs
    /// \param app_data AppData struct
    /// \param scancode user input as scancode
    static bool Handle(AppData *app_data, SDL_Scancode scancode);

    InputHandler() = delete;
private:

    /// Handles backsppace input
    /// \param editor_data EditorData struct
    /// \param is_ctrl_pressed an indicator of whether the LCtrl key is pressed
    /// \return will return true on "expect to be rendered",
    // and returns false if nothing will be changed
    static bool Handle_Backspace(EditorData *editor_data, bool is_ctrl_pressed);

    /// Handles the LCtrl binding of backspace
    /// \param editor_data EditorData struct
    static void Handle_CTRL_Backspace(EditorData *editor_data);
};