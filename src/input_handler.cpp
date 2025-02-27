#include "../inc/input_handler.hpp"
#include "../inc/file_handler.hpp"
#include "../inc/movement.hpp"
#include "../inc/utils.hpp"

#include "../config.hpp"

#define IS_LCTRL SDL_GetModState() & SDL_KMOD_LCTRL


bool
InputHandler::Handle(AppData *app_data, SDL_Scancode scancode)
{
    EditorData *editor_data = &app_data->editor_data;
    EditorMode mode = editor_data->mode;

    switch (scancode) {
    case SDL_SCANCODE_BACKSPACE:
        return Handle_Backspace(editor_data, IS_LCTRL);

    case SDL_SCANCODE_RETURN:
        if (mode == Insert) {
            std::string line = editor_data->file_content[editor_data->cursor.y];
            std::string insert_line;

            if (!Is_Space(line)) {
                insert_line = line.substr(editor_data->cursor.x);
                editor_data->file_content[editor_data->cursor.y]
                    .erase(editor_data->cursor.x);
            } else insert_line = "";
            editor_data->file_content.insert(
                editor_data->file_content.begin() + editor_data->cursor.y + 1,
                insert_line
            );
            editor_data->cursor.y++;
            editor_data->cursor.x = 0;
            return true;
        } break;

    // Movement
    case SDL_SCANCODE_UP:
        return Movement::Move_Cursor_Up(editor_data, IS_LCTRL);
    case SDL_SCANCODE_DOWN:
        return Movement::Move_Cursor_Down(editor_data, IS_LCTRL);
    case SDL_SCANCODE_LEFT:
        return Movement::Move_Cursor_Left(editor_data, IS_LCTRL);
    case SDL_SCANCODE_RIGHT:
        return Movement::Move_Cursor_Right(editor_data, IS_LCTRL);

    case SDL_SCANCODE_H:
        if (mode != Normal && mode != Visual) break;
        return Movement::Move_Cursor_Left(editor_data, IS_LCTRL);
    case SDL_SCANCODE_J:
        if (mode != Normal && mode != Visual) break;
        return Movement::Move_Cursor_Up(editor_data, IS_LCTRL);
    case SDL_SCANCODE_K:
        if (mode != Normal && mode != Visual) break;
        return Movement::Move_Cursor_Down(editor_data, IS_LCTRL);
    case SDL_SCANCODE_L:
        if (mode != Normal && mode != Visual) break;
        return Movement::Move_Cursor_Right(editor_data, IS_LCTRL);

    // Mode
    case SDL_SCANCODE_INSERT:
    case SDL_SCANCODE_I:
        if (mode != Insert) {
            editor_data->mode = Insert;
            SDL_StartTextInput(app_data->window);
            return true;
        } break;

    case SDL_SCANCODE_A:
        if (mode != Insert) {
            if (editor_data->cursor.x < editor_data->file_content[editor_data->cursor.y].length())
                editor_data->cursor.x++;
            editor_data->mode = Insert;
            SDL_StartTextInput(app_data->window);
            return true;
        } break;

    case SDL_SCANCODE_ESCAPE:
        if (mode != Normal) {
            editor_data->mode = Normal;
            SDL_StopTextInput(app_data->window);
            if (
                editor_data->cursor.x >=
                editor_data->file_content[editor_data->cursor.y].length()
            ) editor_data->cursor.x--;
            return true;
        } break;

    // File handling
    case SDL_SCANCODE_S:
        if (ctrl_s_save && IS_LCTRL) FileHandler::Save_File(app_data);

    default:
        break;
    }

    return false;
}


bool
InputHandler::Handle_Backspace(EditorData *editor_data, bool is_ctrl_pressed)
{
    // Do nothing if cursor position is 0,0
    if (editor_data->cursor.x <= 0 && editor_data->cursor.y <= 0)
        return false;

    if (is_ctrl_pressed) {
        Handle_CTRL_Backspace(editor_data);
        return true;
    }

    // Merge line if cursor x is 0
    if (editor_data->cursor.x <= 0 && editor_data->cursor.y) {
        // Get the moved line length
        size_t moved_line_len = editor_data->file_content[editor_data->cursor.y].length();

        // Move the line to the previous line
        editor_data->file_content[editor_data->cursor.y - 1]
            .append(editor_data->file_content[editor_data->cursor.y]);
        editor_data->file_content.erase(
            editor_data->file_content.begin() + editor_data->cursor.y
        );

        editor_data->cursor.y--;
        editor_data->cursor.x =
            editor_data->file_content[editor_data->cursor.y].length() - moved_line_len;
        return true;
    }

    // Remove a single character
    editor_data->file_content[editor_data->cursor.y].erase(editor_data->cursor.x - 1, 1);
    editor_data->cursor.x--;
    return true;
}


void
InputHandler::Handle_CTRL_Backspace(EditorData *editor_data)
{
    while (
        editor_data->cursor.x &&
        Is_Word_Bound(
            editor_data->file_content
            [editor_data->cursor.y]
            [editor_data->cursor.x]
        )
    ) {
        editor_data->file_content[editor_data->cursor.y]
            .erase(editor_data->cursor.x - 1, 1);
        editor_data->cursor.x--;
    }

    while (
        editor_data->cursor.x &&
        !Is_Word_Bound(
            editor_data->file_content
            [editor_data->cursor.y]
            [editor_data->cursor.x]
        )
    ) {
        editor_data->file_content[editor_data->cursor.y]
            .erase(editor_data->cursor.x - 1, 1);
        editor_data->cursor.x--;
    }
}