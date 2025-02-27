#pragma once

#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL.h>

#include <string>

// Colours
#define COL_RESET "\e[0;0;0m"
#define COL_ERR "\e[1;31m"
#define COL_INFO "\e[1;32m"
#define COL_DEBUG "\e[1;36m"
#define COL_WHITE "\e[1;37m"

#define DEBUG_LABEL "\e[1;37m[\e[1;36mDEBUG\e[1;37m]:\e[0;0;0m"
#define INFO_LABEL "\e[1;37m[\e[1;32mINFO\e[1;37m]:\e[0;0;0m"
#define ERR_LABEL "\e[1;37m[\e[1;31mERROR\e[1;37m]:\e[0;0;0m"


enum Direction {
    All,
    Left,
    Right,
};


/// Trims a string
/// \param trim_direction 1 for left, 2 for right, 0 for all
/// \return returns true of success, else returns false
bool Trim_String(std::string &str, Direction trim_direction = All);
SDL_Color Hex_To_Color(std::string &hex);

/// Checks if a string is only composed of alphabet characters (a-zA-Z)
/// \param str checked string
/// \return returns true if all chars of a string is an alpha, else, will return false
bool Is_Alpha(std::string str);

/// Returns font's widht and height
/// \param font TTF_Font object
/// \param w width
/// \param h height
/// \return returns true if success else, returns false
bool Get_Font_Size(TTF_Font *font, int32_t *w, int32_t *h);

/// Logs error to SDL_Log
/// \param fmt c-styled format string / message
/// \param ... additional paramerer for format string
/// \warning this function calls SDL_GetError by itself
__attribute__((__format__ (__printf__, 1, 2)))
void Log_Err(const char *fmt, ...);

/// Logs info to SDL_Log
/// \param fmt c-styled format string / message
/// \param ... additional paramerer for format string
__attribute__((__format__ (__printf__, 1, 2)))
void Log_Info(const char *fmt, ...);

/// Logs debug info to SDL_Log
/// \param fmt c-styled format string / message
/// \param ... additional paramerer for format string
__attribute__((__format__ (__printf__, 1, 2)))
void Log_Debug(const char *fmt, ...);

/// Invert color
/// \param color color to be inverted
/// \return will return an SDL_Color of the inverted color
SDL_Color Invert_Color(SDL_Color color);

/// Checks if the whole str is a whitespace char
/// \param str string to be checked
/// \return true on true, false on false
bool Is_Space(std::string str);

/// Get the width of the string for a font
/// \param font the font to query
/// \param text text to calculate
/// \param w will be filled with the width in pixel
bool Get_String_Width(TTF_Font *font, std::string text, int32_t *w);

/// Checks if the input char is a word bound
/// \param c input char
bool Is_Word_Bound(char c);