#pragma once

#include <SDL3/SDL.h>

/* Colours for the editor (in rgba) */

    // Texts and such
    static const SDL_Color foreground = {255, 255, 255, 255};

    // Unfocused texeds
    static const SDL_Color alt_foreground = {119, 119, 119, 255};

    // Background of the whole editor
    static const SDL_Color background = {14, 14, 14, 255};

    // Colour of the border
    static const SDL_Color border = {0, 255, 255, 255};

    // Colour of unfocused border
    static const SDL_Color alt_border = alt_foreground;

    // Colour of the cursor
    static const SDL_Color cursor = border;

    static const SDL_Color scrollbar_bar = background;
    static const SDL_Color scrollbar_active = {85, 85, 85, 255};
    static const SDL_Color scrollbar_border = border;
    static const SDL_Color scrollbar_alt_border = alt_border;

/* Fonts */

    // Absolute file path of the font file
    // (stuff would probably break if you didnt use a monospace font)
    static const char *font_file = "font/JetBrainsMonoNerdFont-Regular.ttf";

    // Font size
    static const float font_size = 24.0F;

/* Cursors */

    // Cursor width (used for beam type cursor)
    static const float cursor_width = 2.0F;

/* Scrolling */

    // Scroll multiplier for the mouse wheel
    static const float scroll_multiplier = 2.0F;

    // Shows starting open curly braces line when scrolling
    static bool sticky_scroll = true;

/* Decorations */

    // Shows decorations or not
    static bool show_decorations = true;

    // Decoration border colour goes pale when unfocused
    static bool decoration_focus = true;

    // Shows file name decoration
    static bool show_file_name = true;

    // Shown file name would be actual file name and not relative path
    static bool file_name_actual = true;

    // Show vertical scrollbar
    static bool show_v_scrollbar = true;

    // Vertical scrollbar posititon (1 as right, -1 as left)
    static const int8_t v_scrollbar_pos = 1;

    // Vertical scrollbar width in pixel
    static const uint16_t v_scrollbar_width = 10;

/* Line numbers */

    // Line number starts from 0
    static bool zero_indexing = false;

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
    static const int tab_width = 4;

/* Window */

    // Initial window size when spawned in pixels
    static const int initial_window_width = 800;
    static const int initial_window_height = 600;

    // Force window to float and not able to be resized
    static const bool window_always_floating = false;

/* Editor */

    // Text wrapping
    static const bool text_wrapping = false;

/* Rendering */

    // Caching
    // 0 No caching
    // 1 Cache
    static const uint8_t cache_level = 1;