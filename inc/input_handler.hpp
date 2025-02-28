#pragma once

#include "app_data.hpp"


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

    /// Handles the return key / entery key
    /// \param editor_data EditorData struct
    /// \return will return true on "expect to be rendered"
    static bool Handle_Return(EditorData *editor_data);

    /// Handles switching editor mode
    /// \param app_data AppData struct
    /// \param scancode SDL_Scancode type
    static bool Handle_Mode_Switching(AppData *app_data, SDL_Scancode scancode);
};