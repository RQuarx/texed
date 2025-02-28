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


// void
// Log_Err(const char *fmt, ...)
// {
//     time_t now = time(NULL);
//     struct tm log_time;
//     localtime_r(&now, &log_time);

//     char time_buffer[20];
//     strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S", &log_time);

//     char *log_message = nullptr;
//     va_list args;
//     va_start(args, fmt);

//     if (vasprintf(&log_message, fmt, args) < 0)
//         log_message = nullptr;

//     va_end(args);

//     if (log_message) {
//         SDL_Log(
//             "%s %s %s: %s",
//             time_buffer,
//             ERR_LABEL,
//             log_message,
//             SDL_GetError()
//         );
//         free(log_message);
//     } else {
//         SDL_Log(
//             "%s %s %s",
//             time_buffer,
//             ERR_LABEL,
//             SDL_GetError()
//         );
//     }
// }


// void
// Log_Info(const char *fmt, ...)
// {
//     time_t now = time(NULL);
//     struct tm log_time;
//     localtime_r(&now, &log_time);

//     char time_buffer[20];
//     strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S", &log_time);

//     char *log_message = nullptr;
//     va_list args;
//     va_start(args, fmt);

//     if (vasprintf(&log_message, fmt, args) < 0)
//         log_message = nullptr;

//     va_end(args);

//     if (log_message) {
//         SDL_Log(
//             "%s %s %s",
//             time_buffer,
//             INFO_LABEL,
//             log_message
//         );
//         free(log_message);
//     }
// }


// void
// Log_Debug(const char *fmt, ...)
// {
//     time_t now = time(NULL);
//     struct tm log_time;
//     localtime_r(&now, &log_time);

//     char time_buffer[20];
//     strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S", &log_time);

//     char *log_message = nullptr;
//     va_list args;
//     va_start(args, fmt);

//     if (vasprintf(&log_message, fmt, args) < 0)
//         log_message = nullptr;

//     va_end(args);

//     if (log_message) {
//         SDL_Log(
//             "%s %s %s",
//             time_buffer,
//             DEBUG_LABEL,
//             log_message
//         );
//         free(log_message);
//     }
// }