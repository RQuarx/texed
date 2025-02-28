#include "../inc/decoration.hpp"
// #include "../inc/log_utils.hpp"
#include "log_utils.cpp"
#include "../inc/utils.hpp"

#include "../config.hpp"


bool
Decoration::Draw_Decoration(AppData *app_data, Offset *offset)
{
    return Draw_File_Name(app_data, offset);
}


bool
Decoration::Draw_File_Name(AppData *app_data, Offset *offset)
{
    const int32_t padding_value = 10;
    int32_t string_size[2];

    if (
        !TTF_GetStringSize(
            app_data->font,
            app_data->editor_data.file_name.c_str(),
            0,
            &string_size[0],
            &string_size[1]
        )
    ) {
        Log_Err("Failed to get font height");
        return false;
    }

    if (
        !Editor::Render_Text(
            app_data,
            app_data->editor_data.file_name,
            { padding_value, padding_value },
            foreground
        )
    ) return false;

    int32_t y = string_size[1] + (padding_value * 2);

    if (
        !SDL_SetRenderDrawColor(
            app_data->renderer,
            border.r,
            border.g,
            border.b,
            border.a
        )
    ) {
        Log_Err("Failed to set render color");
        return false;
    }

    if (
        !SDL_RenderLine(
            app_data->renderer,
            0,
            y,
            string_size[0] + (padding_value * 2),
            y
        )
    ) {
        Log_Err("Failed to render line");
        return false;
    }

    offset->y = y + padding_value;

    return true;
}