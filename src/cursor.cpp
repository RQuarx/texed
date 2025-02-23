#include "../inc/cursor.hpp"
#include "../inc/utils.hpp"

#include "../config.hpp"


SDL_FRect
RenderCursor::Get_Cursor_Rect(
    AppData *AppData,
    SDL_Color color,
    float x,
    float y,
    float w,
    float h
)
{
    SDL_SetRenderDrawColor(
        AppData->renderer,
        color.r,
        color.g,
        color.b,
        color.a
    );

    return {
        x,
        y,
        (float)w,
        (float)h,
    };
}


bool
RenderCursor::Render(
    AppData *AppData,
    SDL_Color color,
    float x,
    float y,
    float w,
    float h
)
{
    SDL_FRect cursor = Get_Cursor_Rect(AppData, color, x, y, w, h);

    SDL_Surface *surface =
        SDL_CreateSurface(w, h, SDL_PIXELFORMAT_XRGB32);

    if (!surface) {
        Log_Err("Failed to create cursor surface");
        return false;
    }

    uint32_t cursor_color =
        SDL_MapSurfaceRGBA(surface, color.r, color.g, color.b, color.a);

    if (!SDL_FillSurfaceRect(surface, NULL, cursor_color)) {
        Log_Err("Failed to fill cursor surface");
        return false;
    }

    SDL_Texture *texture =
        SDL_CreateTextureFromSurface(AppData->renderer, surface);

    if (!texture) {
        Log_Err("Failed to create texture");
        return false;
    }

    if (!SDL_RenderTexture(AppData->renderer, texture, NULL, &cursor)) {
        Log_Err("Failed to render cursor");
        return false;
    }

    SDL_DestroyTexture(texture);
    SDL_DestroySurface(surface);

    return true;
}


bool
RenderCursor::Hollow(SDL_Color color, AppData *AppData, Offset Offset)
{
    int32_t font_height;
    int32_t font_width;

    Get_Font_Size(AppData->font, &font_width, &font_height);

    float cursor_y = Offset.y + (AppData->EditorData.cursor.y * font_height);
    float cursor_x = Offset.x + (AppData->EditorData.cursor.x * font_width);

    return Render(
        AppData,
        color,
        cursor_x,
        cursor_y,
        (float)font_width,
        (float)font_height
    );
}