#include "../inc/input_handler.hpp"
#include "../inc/movement.hpp"


bool
InputHandler::Handle(_EditorData *EditorData, SDL_Scancode scancode)
{
    switch (scancode) {
    case SDL_SCANCODE_BACKSPACE:
        return Handle_Backspace(EditorData, SDL_GetModState() & SDL_KMOD_LCTRL);
    case SDL_SCANCODE_UP:
        return Movement::Move_Cursor_Up(EditorData, SDL_GetModState() & SDL_KMOD_LCTRL);
    case SDL_SCANCODE_DOWN:
        return Movement::Move_Cursor_Down(EditorData, SDL_GetModState() & SDL_KMOD_LCTRL);
    case SDL_SCANCODE_LEFT:
        return Movement::Move_Cursor_Left(EditorData, SDL_GetModState() & SDL_KMOD_LCTRL);
    case SDL_SCANCODE_RIGHT:
        return Movement::Move_Cursor_Right(EditorData, SDL_GetModState() & SDL_KMOD_LCTRL);
    default:
        break;
    }

    return false;
}


bool
InputHandler::Handle_Backspace(_EditorData *EditorData, bool is_ctrl_pressed)
{
    // Do nothing if cursor position is 0,0
    if (!EditorData->cursor.x && !EditorData->cursor.y)
        return false;

    if (is_ctrl_pressed) {
        Handle_CTRL_Backspace(EditorData);
        return true;
    }

    // Merge line if cursor x is 0
    if (!EditorData->cursor.x && EditorData->cursor.y) {
        // Get the moved line length
        size_t moved_line_len = EditorData->file_content[EditorData->cursor.y].length();

        // Move the line to the previous line
        EditorData->file_content[EditorData->cursor.y - 1]
            .append(EditorData->file_content[EditorData->cursor.y]);
        EditorData->file_content.erase(
            EditorData->file_content.begin() + EditorData->cursor.y
        );

        EditorData->cursor.y--;
        EditorData->cursor.x =
            EditorData->file_content[EditorData->cursor.y].length() - moved_line_len;
        return true;
    }

    // Remove a single character
    EditorData->file_content[EditorData->cursor.y].erase(EditorData->cursor.x - 1, 1);
    EditorData->cursor.x--;
    return true;
}


void
InputHandler::Handle_CTRL_Backspace(_EditorData *EditorData)
{
    while (
        EditorData->cursor.x &&
        std::isspace(
            EditorData->file_content
            [EditorData->cursor.y]
            [EditorData->cursor.x - 1]
        )
    ) {
        EditorData->file_content[EditorData->cursor.y]
            .erase(EditorData->cursor.x - 1, 1);
        EditorData->cursor.x--;
    }

    while (
        EditorData->cursor.x &&
        !std::isspace(
            EditorData->file_content
            [EditorData->cursor.y]
            [EditorData->cursor.x - 1]
        )
    ) {
        EditorData->file_content[EditorData->cursor.y]
            .erase(EditorData->cursor.x - 1, 1);
        EditorData->cursor.x--;
    }
}