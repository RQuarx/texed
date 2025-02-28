#pragma once

#include <SDL3/SDL.h>

#include <string>
#include <array>

struct CommandData {
    std::string content;
    int32_t cursor_pos = 1;
};

static std::array<const char*, 3>command_list = {
    "w",
    "wq",
    "q!"
};


class
CommandHandler
{
public:
    /// Starts the command input and processing process
    /// \param app_data AppData struct
    /// \return false on failure and true on success
    static bool Start_Command(struct AppData *app_data);

    bool Render_Command_Field(struct AppData *app_data);
    CommandHandler() = delete;
};