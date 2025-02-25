#pragma once

#include <SDL3/SDL.h>

static SDL_Color foreground = {255, 255, 255, 255};
static SDL_Color alt_foreground = {119, 119, 119, 255};
static SDL_Color background = {14, 14, 14, 255};
static SDL_Color border = {0, 255, 255, 255};
static SDL_Color cursor = border;

static uint8_t tab_width = 4; // in spaces

static const char *font_file = "/usr/share/fonts/TTF/JetBrainsMonoNerdFont-Regular.ttf";
static float cursor_width = 2.0f;

static float scroll_multiplier = 2.0f;