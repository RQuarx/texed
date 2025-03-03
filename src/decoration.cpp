#include <SDL3_ttf/SDL_ttf.h>

#include "../inc/decoration.hpp"
#include "../inc/utils.hpp"
#include "log_utils.cpp"

#include "../config.hpp"

#include <array>


bool
Decoration::Draw_Decoration(AppData *app_data, Offset *offset)
{
    if (show_file_name && !Draw_File_Name(app_data, offset))
        return false;
    if (show_v_scrollbar && !Draw_Vertical_ScrollBar(app_data, offset))
        return false;

    return true;
}


bool
Decoration::Draw_File_Name(AppData *app_data, Offset *offset)
{
    const int32_t padding_value = 10;
    std::array<int32_t, 2> string_size;
    std::string file_name = (
        file_name_actual ?
        app_data->editor_data.file_path.filename() :
        app_data->editor_data.file_path.relative_path()
    );

    if (
        !TTF_GetStringSize(
            app_data->font,
            file_name.c_str(),
            0,
            string_size.data(),
            &string_size[1]
        )
    ) {
        Log_Err("Failed to get font height");
        return false;
    }

    if (
        !Editor::Render_Text(
            app_data,
            file_name,
            { padding_value + offset->x, padding_value + offset->y },
            foreground
        )
    ) return false;

    int32_t y = string_size[1] + (padding_value * 2) + offset->y;

    bool use_alt_border = decoration_focus && !app_data->focused;
    SDL_Color border_color = (use_alt_border ? alt_border : border);

    if (!Set_Draw_Color(app_data->renderer, border_color)) {
        Log_Err("Failed to set draw colour");
        return false;
    }

    if (
        !SDL_RenderLine(
            app_data->renderer,
            offset->x,
            y,
            string_size[0] + (padding_value * 2) + offset->x,
            y
        )
    ) {
        Log_Err("Failed to render line");
        return false;
    }

    offset->y = y + padding_value;
    return true;
}


bool
Decoration::Draw_Vertical_ScrollBar(AppData *app_data, Offset *offset)
{
    int32_t w;
    int32_t h;
    float x;
    float y = 0.0F;
    if (!SDL_GetWindowSizeInPixels(app_data->window, &w, &h)) {
        Log_Err("Failed to get window size");
        return false;
    }

    x = (v_scrollbar_pos == 1 ? w - v_scrollbar_width : 0.0F);
    w = v_scrollbar_width;

    SDL_FRect scrollbar = {
        x,
        y - 1,
        (float)w,
        (float)h + 1
    };

    bool use_alt_border = decoration_focus && !app_data->focused;
    SDL_Color border_color =
        (use_alt_border ? scrollbar_alt_border : scrollbar_border);

    if (
        !Draw_Border_Rect(
            app_data->renderer,
            &scrollbar,
            scrollbar_bar,
            border_color
        )
    ) return false;

    float file_size = app_data->editor_data.file_content.size();

    if (file_size > 0) {
        float start = (app_data->editor_data.scroll.y / file_size);
        float end = (app_data->editor_data.last_rendered_line / file_size);

        start = std::fmax(0.0F, std::fmin(start, 1.0F));
        end = std::fmax(0.0F, std::fmin(end, 1.0F));

        scrollbar.y = start * h;
        scrollbar.h = (end - start) * h;
    }

    if (v_scrollbar_pos == -1) offset->x += v_scrollbar_width;

    return Draw_Border_Rect(
        app_data->renderer,
        &scrollbar,
        scrollbar_active,
        border_color
    );
}