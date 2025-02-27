#pragma once

#include <SDL3/SDL.h>

/* Colours for the editor */

    // Texts and such
    static SDL_Color foreground = {255, 255, 255, 255};

    // Unfocused texeds
    static SDL_Color alt_foreground = {119, 119, 119, 255};

    // Background of the whole editor
    static SDL_Color background = {14, 14, 14, 255};

    // Colour of the border
    static SDL_Color border = {0, 255, 255, 255};

    // Colour of the cursor
    static SDL_Color cursor = border;

/* Fonts */

    // Absolute file path of the font file
    static const char *font_file = "/usr/share/fonts/TTF/JetBrainsMonoNerdFont-Regular.ttf";

    // Font size
    static const float font_size = 24.0f;

/* Cursors */

    // Cursor width (used for beam type cursor)
    static float cursor_width = 2.0f;

/* Scrolling */

    // Scroll multiplier for the mouse wheel
    static float scroll_multiplier = 2.0f;

    // Shows starting open curly braces line when scrolling
    static bool sticky_scroll = true;

/* Line numbers */

    // Line number starts from 0
    static bool zero_indexing = true;

    // Line number are relative to the cursor
    static bool relative_line_number = true;

    // Move the current like number to the left
    static bool current_line_padding = false;

/* File handling */

    // New file name, created when no file name / path are specified
    static const char *new_file_name = "new_file";

    // New file text, the text put into a new file once created, cant be an empty string
    static const char *new_file_text = " ";

    // Ctrl+S can be used to save files
    static const bool ctrl_s_save = true;

    // The width of a tab '\t' in spaces
    static uint8_t tab_width = 4;