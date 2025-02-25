#pragma once

#include <SDL3/SDL.h>

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#define fs std::filesystem
#define nullopt std::nullopt


struct size_c {
    size_t x;
    size_t y;
};

struct VisualData {
    size_c start_pos;
};

struct Cursor {
    size_t x;
    size_t y;
    size_t max_x;
    VisualData visual_data;

    Cursor() : x(0), y(0), max_x(0), visual_data() {};
};

enum EditorMode {
    Insert,
    Normal,
    Visual,
    Command
};

struct _EditorData {
    std::vector<std::string> file_content;
    std::string file_name;

    size_t last_rendered_line;
    size_t scroll_offset;

    Cursor cursor;

    EditorMode mode;

    _EditorData(std::vector<std::string> file_content, std::string file_name) :
    file_content(file_content),
    file_name(file_name),
    last_rendered_line(0),
    scroll_offset(0),
    cursor(),
    mode(Normal) {};
};


struct Offset {
    int64_t x;
    int64_t y;
};

struct Range {
    int64_t start;
    int64_t end;
};

class
Editor
{
public:
    /// Initialise Editor
    /// \param path text file path
    /// \return on success, will return EditorData. And will return nullopt on failure
    static std::optional<_EditorData> Init_Editor(fs::path path);

    /// Loops containing all of the editor's rendering
    /// \param AppData AppData struct
    /// \param offset the offset of the editor's view
    /// \return will return true on success, and false on failure
    static bool Render_Loop(struct AppData *AppData, struct Offset offset);

    /// Renders line number
    /// \param AppData AppData struct
    /// \param line_index current line index
    /// \param y_offset rendered number y offset
    /// \param line_number_width will be given the value of the line number width
    /// \return will return true on success, and false on failure
    static bool Render_Line_Number(
        struct AppData *AppData,
        size_t line_index,
        uint32_t y_offset,
        int32_t &line_number_width,
        bool relative = true,
        bool zero_index = false
    );

    /// Renders text
    /// \param AppData AppData struct
    /// \param text rendered text
    /// \param offset acts more like the coordinate of where the text will be rendered at
    /// \param color the color the text will be rendered with
    /// \return will return true on success, and false on failure
    static bool Render_Text(
        struct AppData *AppData,
        std::string text,
        struct Offset offset,
        SDL_Color color
    );

    /// Renders the text colour to be inverted from start index to end index
    /// \param AppData AppData struct
    /// \param line the line of string to be rendered
    /// \param Offset acts more like the coordinate of where the text will be rendered at
    /// \param color the color of the text, will be inverted for the inverted text
    /// \param start the start index of the text that will be inverted
    /// \param end the end index of the text that will be inverted
    static bool Render_Inverted_Text(
        struct AppData *AppData,
        std::string line,
        struct Offset offset,
        SDL_Color color,
        struct Range range
    );

    /// Renders cursor based on editor mode
    /// \param AppData AppData struct
    /// \param Offset acts like as the primary coordinates,
    //      if offset is not empty, it will use it to render coordinates instead of the cursor positions
    static bool Render_Cursor(struct AppData *AppData, struct Offset offset);

    Editor() = delete;
};