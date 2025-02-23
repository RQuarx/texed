#include "../inc/decoration.hpp"
#include "../inc/utils.hpp"

#include "../config.hpp"


bool
Decoration::Draw_Decoration(AppData *AppData, Offset *offset)
{
    if (!Draw_File_Name(AppData, offset))
        return false;
    return true;
}


bool
Decoration::Draw_File_Name(AppData *AppData, Offset *offset)
{
    const int32_t padding_value = 10;
    int32_t string_size[2];

    if (
        !TTF_GetStringSize(
            AppData->font,
            AppData->EditorData.file_name.c_str(),
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
            AppData,
            AppData->EditorData.file_name,
            { padding_value, padding_value },
            foreground
        )
    ) return false;

    int32_t y = string_size[1] + (padding_value * 2);

    if (
        !SDL_SetRenderDrawColor(
            AppData->renderer,
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
            AppData->renderer,
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