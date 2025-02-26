#include "../inc/movement.hpp"


bool
Movement::Move_Cursor_Right(_EditorData *EditorData, bool is_ctrl_pressed)
{
    size_t line_len =
        EditorData->file_content[EditorData->cursor.y].length();

    if (EditorData->mode == Normal)
        line_len--;

    Cursor *cursor = &EditorData->cursor;

    if (is_ctrl_pressed) {
        if (
            cursor->x == line_len ||
            std::isspace(EditorData->file_content[cursor->y][cursor->x])
        ) cursor->x++;

        while (
            EditorData->cursor.x < line_len &&
            !std::isspace(
                EditorData->file_content[cursor->y][cursor->x]
            )
        ) cursor->x++;
    } else cursor->x++;

    if (cursor->x > line_len) {
        size_t file_size = EditorData->file_content.size() - 1;
        if (cursor->y < file_size) {
            cursor->y++;
            cursor->x = 0;
        } else if (cursor->y == file_size && EditorData->mode == Normal) {
            cursor->x--;
        }
    }

    cursor->max_x = cursor->x;
    return true;
}


bool
Movement::Move_Cursor_Left(_EditorData *EditorData, bool is_ctrl_pressed)
{
    Cursor *cursor = &EditorData->cursor;
    int32_t min = (EditorData->mode == Normal ? 1 : 0);

    if (cursor->x > 0) {
        if (!is_ctrl_pressed) {
            cursor->x--;
            cursor->max_x = cursor->x;
            return true;
        }

        if (
            std::isspace(EditorData->file_content[cursor->y][cursor->x - min])
        ) cursor->x--;

        while (
            cursor->x &&
            !std::isspace(
                EditorData->file_content[cursor->y][cursor->x - min]
            )
        ) cursor->x--;
        return true;
    }

    if (cursor->y) {
        cursor->y--;
        cursor->x = EditorData->file_content[cursor->y].length() - min;
        return true;
    }
    return false;
}


bool
Movement::Move_Cursor_Up(_EditorData *EditorData, bool is_ctrl_pressed)
{
    Cursor *cursor = &EditorData->cursor;

    if (is_ctrl_pressed) {
        if (EditorData->scroll_offset) {
            EditorData->scroll_offset--;
            return true;
        }
        return false;
    }

    if (!cursor->y) return false;

    cursor->y--;

    size_t line_len =
        EditorData->file_content[cursor->y].length() - 1;

    if (cursor->max_x < line_len)
        cursor->x = cursor->max_x;
    else if (cursor->x > line_len)
        cursor->x = line_len;
    return true;
}


bool
Movement::Move_Cursor_Down(_EditorData *EditorData, bool is_ctrl_pressed)
{
    Cursor *cursor = &EditorData->cursor;

    if (cursor->y >= EditorData->file_content.size() - 1) return false;
    if (
        is_ctrl_pressed &&
        EditorData->scroll_offset < EditorData->file_content.size() - 1
    ) {
        EditorData->scroll_offset++;
        return true;
    }

    cursor->y++;

    if (cursor->y > EditorData->last_rendered_line)
        EditorData->scroll_offset++;

    size_t line_len =
        EditorData->file_content[cursor->y].length() - 1;

    if (cursor->max_x < line_len)
        cursor->x = cursor->max_x;
    else if (cursor->x > line_len)
        cursor->x = line_len;
    return true;
}