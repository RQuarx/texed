#include "../inc/input_handler.hpp"


SDL_AppResult
InputHandler::Handle(_EditorData *EditorData, SDL_Scancode scancode)
{
    switch (scancode) {
    case SDL_SCANCODE_BACKSPACE:
        Handle_Backspace(EditorData, SDL_GetModState() | SDL_KMOD_LCTRL);
        break;
    case SDL_SCANCODE_UP:
        break;
    default:
        break;
    }

    return SDL_APP_CONTINUE;
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