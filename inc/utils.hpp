#pragma once

#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL.h>

#include <string>

enum Direction : uint8_t {
    All,
    Left,
    Right,
};


/// Trims a string
/// \param trim_direction options = All, Left, Right
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

/// Invert color
/// \param color color to be inverted
/// \return will return an SDL_Color of the inverted color
SDL_Color Invert_Color(SDL_Color color);

/// Checks if the whole str is a whitespace char
/// \param str string to be checked
/// \return true on true, false on false
bool Is_Space(std::string_view str);

/// Get the width of the string for a font
/// \param font the font to query
/// \param text text to calculate
/// \param w will be filled with the width in pixel
bool Get_String_Width(TTF_Font *font, const char *text, int32_t *w);

/// Checks if the input char is a word bound
/// \param c input char
bool Is_Word_Bound(char c);

/// Compares str a and b's length
bool StrLen_Compare(std::string_view a, std::string_view b);

/// An easier way to write SDL_SetRenderDrawColor
bool Set_Draw_Color(SDL_Renderer *renderer, SDL_Color color);

/// Draws a filled rects with a border
bool Draw_Border_Rect(
    SDL_Renderer *render,
    SDL_FRect *rect,
    SDL_Color fill_color,
    SDL_Color border_color
);