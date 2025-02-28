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

    switch (scancode) {
    case SDL_SCANCODE_BACKSPACE:
        return Handle_Backspace(editor_data, IS_LCTRL);
    case SDL_SCANCODE_RETURN:
        return Handle_Return(editor_data);

    // Movement
    case SDL_SCANCODE_UP:
    case SDL_SCANCODE_DOWN:
    case SDL_SCANCODE_LEFT:
    case SDL_SCANCODE_RIGHT:
    case SDL_SCANCODE_H:
    case SDL_SCANCODE_J:
    case SDL_SCANCODE_K:
    case SDL_SCANCODE_L:
        return Movement::Move_Cursor(scancode, editor_data, IS_LCTRL);

    // Mode
    case SDL_SCANCODE_INSERT:
    case SDL_SCANCODE_I:
    case SDL_SCANCODE_A:
    case SDL_SCANCODE_ESCAPE:
        return Handle_Mode_Switching(app_data, scancode);

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
    if (editor_data->cursor.x <= 0 && editor_data->cursor.y > 0) {
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
        editor_data->cursor.x > 0 &&
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
        editor_data->cursor.x > 0 &&
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


bool
InputHandler::Handle_Return(EditorData *editor_data)
{
    if (editor_data->mode != Insert) return false;

    std::string_view line = editor_data->file_content[editor_data->cursor.y];
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
}


bool
InputHandler::Handle_Mode_Switching(AppData *app_data, SDL_Scancode scancode)
{
    auto *editor_data = &app_data->editor_data;
    auto mode = editor_data->mode;

    switch (scancode) {
        case SDL_SCANCODE_INSERT:
        case SDL_SCANCODE_I:
            if (mode != Insert) {
                editor_data->mode = Insert;
                SDL_StartTextInput(app_data->window);
                return true;
            } break;

        case SDL_SCANCODE_A:
            if (mode != Insert) {
                if (
                    editor_data->cursor.x <
                    (int64_t)editor_data->file_content[editor_data->cursor.y].length()
                ) editor_data->cursor.x++;
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
                    (int64_t)editor_data->file_content[editor_data->cursor.y].length()
                ) editor_data->cursor.x--;
                return true;
            } break;

        default:
            break;
    }

    return false;
}