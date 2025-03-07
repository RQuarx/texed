#pragma once

#include <SDL3/SDL.h>

#include <filesystem>
#include <optional>
#include <utility>
#include <string>
#include <vector>

#define fs std::filesystem
#define nullopt std::nullopt


struct VisualData {
    int64_t start_x;
    int64_t start_y;
};

struct Cursor {
    int64_t x = 0;
    int64_t y = 0;
    int64_t max_x = 0;
    VisualData visual_data;

    Cursor() : visual_data() {};
};

enum EditorMode : uint8_t {
    Insert,
    Normal,
    Visual,
    Command
};

struct Offset {
    int64_t x;
    int64_t y;

    Offset(int64_t _x, int64_t _y) : x(_x), y(_y) {};
};

struct Cache {
    std::string_view cached_text;
    SDL_Surface *surface;
    SDL_Texture *texture;

    Cache() : surface(nullptr), texture(nullptr) {}
    Cache(std::string_view _cached_text, SDL_Surface *_surface, SDL_Texture *_texture) :
        cached_text(_cached_text),
        surface(_surface),
        texture(_texture) {}
};

struct EditorData {
    std::vector<std::string> file_content;
    fs::path file_path;

    size_t last_rendered_line = 0;
    Offset scroll = { 0, 0 };
    Offset offset = { 0, 0 };

    Cursor cursor;
    std::vector<Cache> cache;

    EditorMode mode = Normal;

    EditorData(std::vector<std::string> &file_content, fs::path &file_path) :
        file_content(std::move(file_content)),
        file_path(std::move(file_path)) {}

    EditorData(EditorData *editor_data) :
        file_content(editor_data->file_content),
        file_path(editor_data->file_path) {}
};

struct Range {
    int64_t start;
    int64_t end;

    Range(int64_t _start, int64_t _end) : start(_start), end(_end) {};
};

class
Editor
{
public:
    /// Initialise Editor
    /// \param path text file path
    /// \return on success, will return EditorData. And will return nullopt on failure
    static std::unique_ptr<EditorData> Init_Editor(fs::path &path);

    /// Loops containing all of the editor's rendering
    /// \param app_data AppData struct
    /// \param offset the offset of the editor's view
    /// \return will return true on success, and false on failure
    static bool Render_Loop(struct AppData *app_data);

    /// The main rendering logic
    static bool Render_Line(
        struct AppData *app_data,
        struct Offset offset,
        int64_t line_index,
        size_t line_height,
        bool &cursor_rendered
    );

    /// Renders line number
    /// \param app_data AppData struct
    /// \param line_index current line index
    /// \param y_offset rendered number y offset
    /// \param line_number_width will be given the value of the line number width
    /// \return will return true on success, and false on failure
    static bool Render_Line_Number(
        struct AppData *app_data,
        int64_t line_index,
        uint32_t y_offset,
        int32_t &line_number_width,
        Cache *cache = nullptr
    );

    /// Renders text
    /// \param app_data AppData struct
    /// \param text rendered text
    /// \param offset acts more like the coordinate of where the text will be rendered at
    /// \param color the color the text will be rendered with
    /// \return will return true on success, and false on failure
    static bool Render_Text(
        struct AppData *app_data,
        std::string &text,
        struct Offset offset,
        SDL_Color color,
        Cache *cache = nullptr
    );

    /// Renders the text colour to be inverted from start index to end index
    /// \param app_data AppData struct
    /// \param line the line of string to be rendered
    /// \param Offset acts more like the coordinate of where the text will be rendered at
    /// \param color the color of the text, will be inverted for the inverted text
    /// \param range the start and end range of the inverted text
    static bool Render_Inverted_Text(
        struct AppData *app_data,
        std::string &line,
        struct Offset offset,
        SDL_Color color,
        struct Range range
    );

    /// Renders cursor based on editor mode
    /// \param app_data AppData struct
    /// \param Offset acts like as the primary coordinates,
    //      if offset is not empty, it will use it to render coordinates instead of the cursor positions
    static bool Render_Cursor(struct AppData *app_data, struct Offset offset);

    Editor() = delete;
};

