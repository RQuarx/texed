#include "../inc/app_data.hpp"
#include "../inc/cursor.hpp"
#include "../inc/editor.hpp"
#include "../inc/utils.hpp"

#include "../config.hpp"

#include <fstream>
#include <string>
#include <cmath>


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

    // Used to make it only render the necesarry lines
    const int32_t line_height = TTF_GetFontHeight(AppData->font);
    int32_t window_height;

    if (
        !SDL_GetWindowSizeInPixels(
            AppData->window,
            nullptr,
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
    size_t i = EditorData->scroll_offset;

    loop {
        if (i >= EditorData->last_rendered_line) break;
        /*
        Renders line index and
        gets the width of the line index for the text offset
        */
        size_t line_index =
            EditorData->scroll_offset + (y_offset - offset.y) / line_height;
        if (line_index >= EditorData->last_rendered_line) break;

        int32_t line_number_width;
        Render_Line_Number(AppData, i, y_offset, line_number_width);
        line_number_width += offset.x;

        int32_t text_padding;

        if (
            !TTF_GetStringSize(
                AppData->font,
                "  ",
                0,
                &text_padding,
                nullptr
            )
        ) {
            Log_Err("Failed to get text padding size");
            return false;
        }

        // Renders editor text
        if (
            !Render_Text(
                AppData,
                EditorData->file_content[i],
                {(uint32_t)line_number_width + text_padding, y_offset},
                foreground
            )
        ) return false;

        y_offset += line_height;
        i++;

        if (
            EditorData->cursor.y <= EditorData->last_rendered_line &&
            !Render_Cursor(
                AppData,
                {
                    offset.x + line_number_width + text_padding,
                    offset.y
                }
            )
        ) return false;
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
    if (!RenderCursor::Hollow(cursor, AppData, Offset)) {
        Log_Err("Failed to render cursor");
        return false;
    }
    return true;
}