#include "../inc/app_data.hpp"
#include "../inc/cursor.hpp"
#include "../inc/editor.hpp"
#include "../inc/utils.hpp"
#include "log_utils.cpp"

#include "../config.hpp"

#include <fstream>
#include <string>
#include <cmath>
#include <array>


std::unique_ptr<EditorData>
Editor::Init_Editor(fs::path &path)
{
    if (!fs::is_regular_file(path))
        return nullptr;

    std::vector<std::string> file_content;

    std::ifstream text_file(path);
    std::string line;

    while (std::getline(text_file, line)) {
        if (line == "\t") line = std::string(tab_width, ' ');
        else if (!Trim_String(line, Right)) line = "";
        file_content.push_back(line);
    }

    text_file.close();

    std::unique_ptr<EditorData> editor_data =
        std::make_unique<EditorData>(file_content, path);
    editor_data->cache.reserve(editor_data->file_content.size());

    return editor_data;
}


bool
Editor::Render_Loop(AppData *app_data)
{
    EditorData *editor_data = &app_data->editor_data;
    int32_t line_height = TTF_GetFontHeight(app_data->font);
    int32_t window_height = 0;
    bool cursor_rendered = false;

    if (
        !SDL_GetWindowSizeInPixels(
            app_data->window,
            nullptr,
            &window_height
        )
    ) {
        Log_Err("Failed to get window size");
        return false;
    }

    editor_data->last_rendered_line = std::min(
        editor_data->scroll.y + ((window_height - editor_data->offset.y) / line_height),
        (int64_t)editor_data->file_content.size()
    );

    for (
        int64_t i = editor_data->scroll.y, y_offset = 0;
        i < (int64_t)editor_data->last_rendered_line + 1;
        i++, y_offset += line_height
    ) {
        if (
            !Render_Line(
                app_data,
                { editor_data->offset.x, editor_data->offset.y + y_offset },
                i,
                line_height,
                cursor_rendered
            )
        ) return false;
    }

    return true;
}


bool
Editor::Render_Line(
    AppData *app_data,
    Offset offset,
    int64_t line_index,
    size_t line_height,
    bool &cursor_rendered
)
{
    EditorData *editor_data = &app_data->editor_data;
    Cursor *cursor = &editor_data->cursor;

    std::string line = (
        line_index < (int64_t)editor_data->file_content.size() ?
        editor_data->file_content[line_index] :
        ""
    );

    if (editor_data->scroll.x >= (int64_t)line.length()) line = "";
    else line = line.substr(editor_data->scroll.x);

    Offset render_offset(offset.x, offset.y);
    int32_t line_number_width;
    int32_t text_padding;

    if (
        !Render_Line_Number(app_data, line_index, offset.y, line_number_width) ||
        !Get_String_Width(app_data->font, "  ", &text_padding)
    ) return false;
    render_offset.x += line_number_width + text_padding;

    if (
        !cursor_rendered &&
        cursor->y <= (int64_t)editor_data->last_rendered_line &&
        cursor->y >= editor_data->scroll.y
    ) {
        int64_t cursor_y = offset.y - (editor_data->scroll.y * line_height);
        if (!Render_Cursor(app_data, { render_offset.x, cursor_y })) return false;
        cursor_rendered = true;
    }

    if (
        app_data->focused &&
        app_data->editor_data.mode == Normal &&
        cursor->y == line_index
    ) {
        if (
            !Render_Inverted_Text(
                app_data,
                line,
                render_offset,
                foreground,
                Range(cursor->x, cursor->x)
            )
        ) return false;
    } else {
        while (cache_level == 1 && (int64_t)editor_data->cache.size() <= line_index)
            editor_data->cache.emplace_back();

        if (
            !Render_Text(
                app_data,
                line,
                render_offset,
                foreground,
                (cache_level == 1 ? &editor_data->cache[line_index] : nullptr)
            )
        ) return false;
    }
    return true;
}


bool
Editor::Render_Text(
    AppData *app_data,
    std::string &text,
    Offset offset,
    SDL_Color color,
    Cache *cache
)
{
    bool should_cache = (
        cache == nullptr || text != cache->cached_text ||
        (cache->texture == nullptr || cache->surface == nullptr)
    );
    SDL_Surface *surface = nullptr;
    SDL_Texture *texture = nullptr;

    if (should_cache) {
        surface = TTF_RenderText_Blended(
            app_data->font,
            (text.empty() ? " " : text.data()),
            0,
            color
        );

        if (surface == nullptr) {
            Log_Err("Failed to create surface");
            return false;
        }

        texture = SDL_CreateTextureFromSurface(app_data->renderer, surface);

        if (texture == nullptr) {
            Log_Err("Failed to create texture from surface");
            SDL_DestroySurface(surface);
            return false;
        }

        if (cache != nullptr) {
            if (cache->surface != nullptr) SDL_DestroySurface(cache->surface);
            if (cache->texture != nullptr) SDL_DestroyTexture(cache->texture);

            cache->surface = surface;
            cache->texture = texture;
            cache->cached_text = text;
        }
    } else {
        surface = cache->surface;
        texture = cache->texture;
    }

    SDL_FRect rect = {
        (float)offset.x,
        (float)offset.y,
        (float)surface->w,
        (float)surface->h
    };

    if (!SDL_RenderTexture(app_data->renderer, texture, nullptr, &rect)) {
        Log_Err("Failed to render text texture");
        return false;
    }

    if (cache == nullptr) {
        SDL_DestroySurface(surface);
        SDL_DestroyTexture(texture);
    }

    return true;
}


bool
Editor::Render_Inverted_Text(
    AppData *app_data,
    std::string &line,
    Offset Offset,
    SDL_Color color,
    Range range
)
{
    if (line.empty()) line = " ";
    if (
        range.start > (int64_t)line.size() ||
        range.end >= (int64_t)line.size() ||
        range.start > range.end
    ) {
        Log_Err("Invalid start or end value: {} {}", range.start, range.end);
        return false;
    }

    std::array<std::string, 3> text = {
        (range.start > 0 ? line.substr(0, range.start) : ""),
        line.substr(range.start, (range.end + 1) - range.start),
        (range.end >= (int64_t)line.length() ? "" : line.substr(range.end + 1))
    };

    std::array<int32_t, 3> str_offset = {0};

    for (size_t i = 0; i < text.size(); i++) {
        if (
            i != 2 &&
            !Get_String_Width(app_data->font, text[i].c_str(), &str_offset[i + 1])
        ) return false;
        str_offset[i] += (i > 0 ? str_offset[i - 1] : Offset.x);

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
    int32_t &line_number_width,
    Cache *cache
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
            ),
            cache
        )
    ) return false;

    if (
        TTF_GetStringSize(
            app_data->font,
            line_number.c_str(),
            0,
            &line_number_width,
            nullptr
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