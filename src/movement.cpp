#include "../inc/movement.hpp"
#include "../inc/utils.hpp"


bool
Movement::Move_Cursor_Right(EditorData *editor_data, bool is_ctrl_pressed)
{
    int64_t line_len =
        editor_data->file_content[editor_data->cursor.y].length();
    if (editor_data->mode == Normal && line_len > 0) line_len--;

    Cursor *cursor = &editor_data->cursor;

    if (!is_ctrl_pressed && cursor->x < line_len) {
        cursor->x++;
        cursor->max_x = cursor->x;
        return true;
    }

    if (is_ctrl_pressed && cursor->x < line_len) {
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

    if (!is_ctrl_pressed && cursor->x > 0) {
        cursor->x--;
        cursor->max_x = cursor->x;
        return true;
    }

    if (is_ctrl_pressed && cursor->x > 0) {
        std::string line = editor_data->file_content[cursor->y];
        if (
            cursor->x > 0 &&
            Is_Word_Bound(line[editor_data->cursor.x - min])
        ) cursor->x--;

        while (
            cursor->x > 0 &&
            !Is_Word_Bound(line[editor_data->cursor.x - min])
        ) cursor->x--;
        cursor->max_x = cursor->x;
        return true;
    }

    if (cursor->y > 0 && cursor->x <= 0) {
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
        if (editor_data->scroll.y > 0) {
            editor_data->scroll.y--;
            return true;
        }
        return false;
    }

    if (cursor->y <= 0) return false;
    editor_data->scroll.y = std::min(--cursor->y, editor_data->scroll.y);

    int64_t line_len =
        editor_data->file_content[cursor->y].length();
    if (editor_data->mode == Normal && line_len > 0)
        line_len--;

    cursor->x = cursor->max_x;
    cursor->x = std::min(cursor->x, line_len);
    return true;
}


bool
Movement::Move_Cursor_Down(EditorData *editor_data, bool is_ctrl_pressed)
{
    Cursor *cursor = &editor_data->cursor;

    if (cursor->y >= (int64_t)editor_data->file_content.size() - 1) return false;
    if (
        is_ctrl_pressed &&
        editor_data->scroll.y < (int64_t)editor_data->file_content.size() - 1
    ) {
        editor_data->scroll.y++;
        return true;
    }

    if (++cursor->y >= (int64_t)editor_data->last_rendered_line) {
        while (cursor->y >= (int64_t)editor_data->last_rendered_line) {
            editor_data->last_rendered_line++;
            editor_data->scroll.y++;
        }
    }
    editor_data->scroll.y = std::min(cursor->y, editor_data->scroll.y);

    int64_t line_len =
        editor_data->file_content[cursor->y].length();
    if (editor_data->mode == Normal && line_len > 0) line_len--;

    cursor->x = cursor->max_x;
    cursor->x = std::min(cursor->x, line_len);
    return true;
}


bool
Movement::Move_Cursor(
    SDL_Scancode scancode, EditorData *editor_data, bool is_ctrl_pressed
)
{
    switch (scancode) {
    case SDL_SCANCODE_UP:
        return Movement::Move_Cursor_Up(editor_data, is_ctrl_pressed);
    case SDL_SCANCODE_DOWN:
        return Movement::Move_Cursor_Down(editor_data, is_ctrl_pressed);
    case SDL_SCANCODE_LEFT:
        return Movement::Move_Cursor_Left(editor_data, is_ctrl_pressed);
    case SDL_SCANCODE_RIGHT:
        return Movement::Move_Cursor_Right(editor_data, is_ctrl_pressed);

    case SDL_SCANCODE_H:
        if (editor_data->mode != Normal && editor_data->mode != Visual) break;
        return Movement::Move_Cursor_Left(editor_data, is_ctrl_pressed);
    case SDL_SCANCODE_J:
        if (editor_data->mode != Normal && editor_data->mode != Visual) break;
        return Movement::Move_Cursor_Up(editor_data, is_ctrl_pressed);
    case SDL_SCANCODE_K:
        if (editor_data->mode != Normal && editor_data->mode != Visual) break;
        return Movement::Move_Cursor_Down(editor_data, is_ctrl_pressed);
    case SDL_SCANCODE_L:
        if (editor_data->mode != Normal && editor_data->mode != Visual) break;
        return Movement::Move_Cursor_Right(editor_data, is_ctrl_pressed);
    default:
        break;
    }
    return false;
}