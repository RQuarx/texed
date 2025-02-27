#include "../inc/app_data.hpp"
#include "../inc/cursor.hpp"
#include "../inc/editor.hpp"
#include "../inc/utils.hpp"

#include "../config.hpp"

#include <fstream>
#include <string>
#include <cmath>
#include <array>


std::optional<EditorData>
Editor::Init_Editor(fs::path path)
{
    if (!fs::is_regular_file(path))
        return nullopt;

    std::vector<std::string> file_content;

    std::ifstream text_file(path);
    std::string line;

    while (std::getline(text_file, line)) {
        if (line == "\t") line = std::string(tab_width, ' ');
        else if (!Trim_String(line, Right)) line = "";
        file_content.push_back(line);
    }

    text_file.close();

    return EditorData(
        file_content,
        (path.has_relative_path() ? path.relative_path() : "no_name")
    );
}


bool
Editor::Render_Loop(AppData *app_data, Offset offset)
{
    EditorData *editor_data = &app_data->editor_data;
    Cursor *cursor = &editor_data->cursor;
    int32_t line_height = TTF_GetFontHeight(app_data->font);
    int32_t window_height = 0;
    bool cursor_rendered = false;

    if (
        !SDL_GetWindowSizeInPixels(
            app_data->window,
            NULL,
            &window_height
        )
    ) {
        Log_Err("Failed to get window size");
        return SDL_APP_FAILURE;
    }

    editor_data->last_rendered_line = std::min(
        editor_data->scroll_offset + (window_height / line_height),
        editor_data->file_content.size()
    );

    // Starts off offset with the input offset
    uint32_t y_offset = offset.y;

    for (size_t i = editor_data->scroll_offset; i < editor_data->last_rendered_line; i++) {
        std::string line = editor_data->file_content[i];
        Offset render_offset = { 0, y_offset };
        int32_t line_number_width;
        int32_t text_padding;

        /*
        Renders line index and
        gets the width of the line index for the text offset
        */
        if (!Render_Line_Number(app_data, i, y_offset, line_number_width)) return false;
        if (!Get_String_Width(app_data->font, "  ", &text_padding)) return false;
        render_offset.x = offset.x + line_number_width + text_padding;

        // Renders cursor
        if (
            !cursor_rendered &&
            cursor->y <= (int64_t)editor_data->last_rendered_line &&
            cursor->y >= (int64_t)editor_data->scroll_offset
        ) {
            int64_t cursor_y = y_offset - (editor_data->scroll_offset * line_height);
            if (!Render_Cursor(app_data, { render_offset.x, cursor_y })) return false;
            cursor_rendered = true;
        }

        // Renders texts
        if (
            app_data->focused &&
            app_data->editor_data.mode == Normal &&
            cursor->y == (int64_t)i
        ) {
            if (
                !Render_Inverted_Text(
                    app_data,
                    line,
                    render_offset,
                    foreground,
                    { cursor->x, cursor->x }
                )
            ) return false;
        } else if (!Render_Text(app_data, line, render_offset, foreground))
            return false;

        y_offset += line_height;
    }

    return true;
}


bool
Editor::Render_Text(
    AppData *app_data,
    std::string text,
    Offset offset,
    SDL_Color color
)
{
    if (text.empty()) text = " ";
    SDL_Surface *surface = TTF_RenderText_Blended(
        app_data->font,
        text.c_str(),
        0,
        color
    );

    if (!surface) {
        Log_Err("Failed to create surface");
        return false;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(app_data->renderer, surface);

    if (!texture) {
        Log_Err("Failed to create texture from surface");
        return false;
    }

    SDL_FRect rect = {
        (float)offset.x,
        (float)offset.y,
        (float)surface->w,
        (float)surface->h
    };

    bool return_val = SDL_RenderTexture(app_data->renderer, texture, NULL, &rect);
    SDL_DestroySurface(surface);
    SDL_DestroyTexture(texture);

    return return_val;
}


bool
Editor::Render_Inverted_Text(
    struct AppData *app_data,
    std::string line,
    struct Offset Offset,
    SDL_Color color,
    struct Range range
)
{
    if (line.empty()) line = " ";
    if (
        range.start > (int64_t)line.size() ||
        range.end >= (int64_t)line.size() ||
        range.start > range.end
    ) {
        Log_Err("Invalid start or end value: %lu %lu", range.start, range.end);
        return false;
    }

    std::array<std::string, 3> text = {
        (range.start ? line.substr(0, range.start) : ""),
        line.substr(range.start, (range.end + 1) - range.start),
        (range.end >= (int64_t)line.length() ? "" : line.substr(range.end + 1))
    };

    std::array<int32_t, 3> str_offset = {0};

    for (uint8_t i = 0; i < text.size(); i++) {
        if (i != 2 && !Get_String_Width(app_data->font, text[i], &str_offset[i + 1]))
            return false;
        str_offset[i] += (i ? str_offset[i - 1] : Offset.x);

        if (
            !text[i].empty() &&
            !Render_Text(
                app_data,
                text[i],
                { (uint32_t)str_offset[i], Offset.y },
                (i == 1 ? Invert_Color(color) : color)
            )
        ) {
            Log_Err("Failed to render text");
            return false;
        }
    }
    return true;
}


bool
Editor::Render_Line_Number(
    AppData *app_data,
    int64_t line_index,
    uint32_t y_offset,
    int32_t &line_number_width
)
{
    int8_t add = (zero_indexing ? 0 : 1);
    std::string line_number = std::to_string(
        relative_line_number ?
        std::abs(app_data->editor_data.cursor.y - line_index) + add - 1 :
        line_index + add
    );

    if (relative_line_number && app_data->editor_data.cursor.y == line_index)
        line_number = std::to_string(line_index + add);

    line_number.insert(
        0,
        std::log10(app_data->editor_data.file_content.size()) + 4 - line_number.length(),
        ' '
    );

    if (app_data->editor_data.cursor.y == line_index && current_line_padding) {
        line_number.erase(0, 2);
        line_number += "  ";
    }

    if (!Render_Text(
            app_data,
            line_number,
            { 0, y_offset },
            (
                app_data->editor_data.cursor.y == line_index ?
                foreground : alt_foreground
            )
        )
    ) return false;

    if (
        TTF_GetStringSize(
            app_data->font,
            line_number.c_str(),
            0,
            &line_number_width,
            NULL
        )
    ) return true;

    Log_Err("Failed to get line number width");
    return false;
}


bool
Editor::Render_Cursor(AppData *app_data, Offset Offset)
{
    if (!app_data->focused) {
        if (!RenderCursor::Hollow(cursor, app_data, Offset)) {
            Log_Err("Failed to render cursor");
            return false;
        }
        return true;
    }

    bool return_code;
    switch (app_data->editor_data.mode) {
    case Insert:
        return_code = RenderCursor::Beam(cursor, app_data, Offset);
        break;
    case Normal:
        return_code = RenderCursor::Box(cursor, app_data, Offset);
        break;
    case Command:
        return_code = RenderCursor::Line(cursor, app_data, Offset);
        break;
    case Visual:
        return_code = RenderCursor::Box(cursor, app_data, Offset);
        break;
    }

    if (!return_code) Log_Err("Failed to render cursor");
    return return_code;
}