#define __STDC_WANT_LIB_EXT1__ 1

#include "../inc/utils.hpp"
#include "log_utils.cpp"

#include <algorithm>
#include <cstdarg>
#include <cstdio>


bool
Trim_String(std::string &str, Direction trim_direction)
{
    if (trim_direction == All || trim_direction == Left)
        str.erase(
            str.begin(),
            std::ranges::find_if(
                str,
                [](unsigned char ch)
                { return !std::isspace(ch); }
            )
        );

    if (trim_direction == All || trim_direction == Right)
        str.erase(
            std::ranges::find_if(
                str.rbegin(),
                str.rend(),
                [](unsigned char ch)
                { return !std::isspace(ch); }
            ).base(), str.end()
        );
    return !str.empty();
}


bool
Is_Alpha(std::string str)
{
    return std::ranges::all_of(
        str,
        [](int8_t c){ return std::isalpha(c); }
    );
}


bool
Get_Font_Size(TTF_Font *font, int32_t *w, int32_t *h)
{
    if (!TTF_GetStringSize(font, "A", 0, w, h)) {
        Log_Err("Failed to get string size");
        return false;
    }
    return true;
}


SDL_Color
Invert_Color(SDL_Color color)
{
    return {
        static_cast<uint8_t>(UINT8_MAX - color.r),
        static_cast<uint8_t>(UINT8_MAX - color.g),
        static_cast<uint8_t>(UINT8_MAX - color.b),
        color.a
    };
}


bool
Is_Space(std::string_view str)
{
    return (
        str.empty() ?
        true :
        std::ranges::all_of(
            str,
            [](uint8_t c){ return std::isspace(c); }
        )
    );
}


bool
Get_String_Width(TTF_Font *font, const char *text, int32_t *w)
{
    if (
        !TTF_GetStringSize(
            font,
            text,
            0,
            w,
            nullptr
        )
    ) {
        Log_Err("Failed to get string width");
        return false;
    }
    return true;
}


bool
Is_Word_Bound(char c)
{ return (std::isspace(c) > 0 || std::ispunct(c) > 0); }


bool
StrLen_Compare(std::string_view a, std::string_view b)
{ return a.length() < b.length(); }


bool
Set_Draw_Color(SDL_Renderer *renderer, SDL_Color color)
{
    return
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}


bool
Draw_Border_Rect(
    SDL_Renderer *render,
    SDL_FRect *rect,
    SDL_Color fill_color,
    SDL_Color border_color
)
{
    bool result = (
        Set_Draw_Color(render, fill_color) &&
        SDL_RenderFillRect(render, rect) &&
        Set_Draw_Color(render, border_color) &&
        SDL_RenderRect(render, rect)
    );

    if (!result) {
        Log_Err("Failed to render bordered rect");
        return false;
    }

    return true;
}