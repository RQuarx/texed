#include "../inc/movement.hpp"
#include "../inc/utils.hpp"


bool
Movement::Move_Cursor_Right(EditorData *editor_data, bool is_ctrl_pressed)
{
    int64_t line_len =
        editor_data->file_content[editor_data->cursor.y].length();
    if (editor_data->mode == Normal)
        line_len--;

    Cursor *cursor = &editor_data->cursor;

    if (!is_ctrl_pressed && cursor->x < line_len) {
        cursor->x++;
        cursor->max_x = cursor->x;
        return true;
    } else if (is_ctrl_pressed && cursor->x < line_len) {
        std::string line = editor_data->file_content[cursor->y];
        if (
            cursor->x <= line_len &&
            Is_Word_Bound(line[cursor->x])
        ) cursor->x++;

        while (
            editor_data->cursor.x < line_len &&
            !Is_Word_Bound(line[cursor->x])
        ) {
            cursor->x++;
        }
        cursor->max_x = cursor->x;
        return true;
    }

    if (cursor->x == line_len) {
        int64_t file_size = editor_data->file_content.size() - 1;
        if (cursor->y < file_size) {
            cursor->y++;
            cursor->x = 0;
        } else if (cursor->y == file_size && editor_data->mode == Normal) {
            cursor->x--;
        }
        return true;
    }
    return false;
}


bool
Movement::Move_Cursor_Left(EditorData *editor_data, bool is_ctrl_pressed)
{
    Cursor *cursor = &editor_data->cursor;
    uint8_t min = (editor_data->mode == Normal ? 1 : 0);

    if (!is_ctrl_pressed && cursor->x) {
        cursor->x--;
        cursor->max_x = cursor->x;
        return true;
    }

    if (is_ctrl_pressed && cursor->x) {
        std::string line = editor_data->file_content[cursor->y];
        if (
            cursor->x &&
            Is_Word_Bound(line[editor_data->cursor.x - min])
        ) cursor->x--;

        while (
            cursor->x &&
            !Is_Word_Bound(line[editor_data->cursor.x - min])
        ) cursor->x--;
        cursor->max_x = cursor->x;
        return true;
    }

    if (cursor->y && cursor->x <= 0) {
        int64_t len = editor_data->file_content[cursor->y - 1].length() - min;
        cursor->y--;
        cursor->x = (len < 0 ? 0 : len);
        cursor->max_x = cursor->x;
        return true;
    }
    return false;
}


bool
Movement::Move_Cursor_Up(EditorData *editor_data, bool is_ctrl_pressed)
{
    Cursor *cursor = &editor_data->cursor;

    if (is_ctrl_pressed) {
        if (editor_data->scroll_offset) {
            editor_data->scroll_offset--;
            return true;
        }
        return false;
    }

    if (!cursor->y) return false;

    cursor->y--;

    int64_t line_len =
        editor_data->file_content[cursor->y].length();
    if (editor_data->mode == Normal)
        line_len--;

    cursor->x = cursor->max_x;
    if (cursor->x > line_len) cursor->x = line_len;
    return true;
}


bool
Movement::Move_Cursor_Down(EditorData *editor_data, bool is_ctrl_pressed)
{
    Cursor *cursor = &editor_data->cursor;

    if (cursor->y >= (int64_t)editor_data->file_content.size() - 1) return false;
    if (
        is_ctrl_pressed &&
        editor_data->scroll_offset < editor_data->file_content.size() - 1
    ) {
        editor_data->scroll_offset++;
        return true;
    }

    cursor->y++;

    if (cursor->y > (int64_t)editor_data->last_rendered_line)
        editor_data->scroll_offset++;

    int64_t line_len =
        editor_data->file_content[cursor->y].length();
    if (editor_data->mode == Normal)
        line_len--;

    cursor->x = cursor->max_x;
    if (cursor->x > line_len) cursor->x = line_len;
    return true;
}