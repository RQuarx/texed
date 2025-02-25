#include "../inc/app_data.hpp"
#include "../inc/cursor.hpp"
#include "../inc/editor.hpp"
#include "../inc/utils.hpp"

#include "../config.hpp"

#include <fstream>
#include <string>
#include <cmath>
#include <array>


std::optional<_EditorData>
Editor::Init_Editor(fs::path path)
{
    if (!fs::is_regular_file(path))
        return nullopt;

    std::vector<std::string> file_content;

    std::ifstream text_file(path);
    std::string line;

    while (std::getline(text_file, line)) {
        if (line == "\t") line = std::string(tab_width, ' ');
        else if (!Trim_String(line, Right)) line = " ";
        file_content.push_back(line);
    }

    text_file.close();

    return _EditorData(
        file_content,
        (path.has_relative_path() ? path.relative_path() : "no_name")
    );
}


bool
Editor::Render_Loop(AppData *AppData, Offset offset)
{
    auto EditorData = &AppData->EditorData;
    auto cursor = &EditorData->cursor;
    int32_t line_height = TTF_GetFontHeight(AppData->font);
    int32_t window_height = 0;
    bool cursor_rendered = false;

    if (
        !SDL_GetWindowSizeInPixels(
            AppData->window,
            NULL,
            &window_height
        )
    ) {
        Log_Err("Failed to get window size");
        return SDL_APP_FAILURE;
    }

    EditorData->last_rendered_line = std::min(
        EditorData->scroll_offset + (window_height / line_height),
        EditorData->file_content.size()
    );

    // Starts off offset with the input offset
    uint32_t y_offset = offset.y;

    for (size_t i = EditorData->scroll_offset; i < EditorData->last_rendered_line; i++) {
        std::string line = EditorData->file_content[i];
        Offset render_offset = { 0, y_offset };
        int32_t line_number_width;
        int32_t text_padding;

        /*
        Renders line index and
        gets the width of the line index for the text offset
        */
        if (!Render_Line_Number(AppData, i, y_offset, line_number_width)) return false;
        if (!Get_String_Width(AppData->font, "  ", &text_padding)) return false;

        render_offset.x = offset.x + line_number_width + text_padding;

        if (
            !cursor_rendered &&
            cursor->y <= EditorData->last_rendered_line &&
            cursor->y >= EditorData->scroll_offset
        ) {
            int64_t cursor_y = y_offset - (EditorData->scroll_offset * line_height);
            if (!Render_Cursor(AppData, { render_offset.x, cursor_y })) return false;
            cursor_rendered = true;
        }

        if (
            AppData->focused &&
            AppData->EditorData.mode == Normal &&
            cursor->y == i
        ) {
            if (
                !Render_Inverted_Text(
                    AppData,
                    line,
                    render_offset,
                    foreground,
                    { (int64_t)cursor->x, (int64_t)cursor->x }
                )
            ) return false;
        } else if (!Render_Text(AppData, line, render_offset, foreground))
            return false;

        y_offset += line_height;
    }

    return true;
}


bool
Editor::Render_Text(
    AppData *AppData,
    std::string text,
    Offset offset,
    SDL_Color color
)
{
    SDL_Surface *surface = TTF_RenderText_Blended(
        AppData->font,
        text.c_str(),
        0,
        color
    );

    if (!surface) {
        Log_Err("Failed to create surface");
        return false;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(AppData->renderer, surface);

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

    bool return_val = SDL_RenderTexture(AppData->renderer, texture, NULL, &rect);
    SDL_DestroySurface(surface);
    SDL_DestroyTexture(texture);

    return return_val;
}


bool
Editor::Render_Inverted_Text(
    struct AppData *AppData,
    std::string line,
    struct Offset Offset,
    SDL_Color color,
    struct Range range
)
{
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
        if (i != 2 && !Get_String_Width(AppData->font, text[i], &str_offset[i + 1]))
            return false;
        str_offset[i] += (i ? str_offset[i - 1] : Offset.x);

        if (
            !text[i].empty() &&
            !Render_Text(
                AppData,
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
    AppData *AppData,
    size_t line_index,
    uint32_t y_offset,
    int32_t &line_number_width,
    bool relative,
    bool zero_index
)
{
    std::string line_number = std::to_string(
        relative ?
        std::abs((int32_t)AppData->EditorData.cursor.y - (int32_t)line_index) :
        (zero_index ? line_index - 1 : line_index)
    );

    line_number.insert(
        0,
        std::log10(AppData->EditorData.file_content.size()) + 4 - line_number.length(),
        ' '
    );

    if (AppData->EditorData.cursor.y == line_index) {
        line_number.erase(0, 2);
        line_number += "  ";
    }

    if (!Render_Text(
            AppData,
            line_number,
            { 0, y_offset },
            (AppData->EditorData.cursor.y == line_index ? foreground : alt_foreground)
        )
    ) return false;

    if (
        TTF_GetStringSize(
            AppData->font,
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
Editor::Render_Cursor(AppData *AppData, Offset Offset)
{
    if (!AppData->focused) {
        if (!RenderCursor::Hollow(cursor, AppData, Offset)) {
            Log_Err("Failed to render cursor");
            return false;
        }
        return true;
    }

    switch (AppData->EditorData.mode) {
    case Insert:
        if (!RenderCursor::Beam(cursor, AppData, Offset)) {
            Log_Err("Failed to render cursor");
            return false;
        }
        break;
    case Normal:
        if (!RenderCursor::Box(cursor, AppData, Offset)) {
            Log_Err("Failed to render cursor");
            return false;
        }
        break;
    case Command:
        if (!RenderCursor::Line(cursor, AppData, Offset)) {
            Log_Err("Failed to render cursor");
            return false;
        }
        break;
    case Visual:
        if (!RenderCursor::Box(cursor, AppData, Offset)) {
            Log_Err("Failed to render cursor");
            return false;
        }
        break;
    }
    return true;
}