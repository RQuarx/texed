#pragma once

#include "arg_parse.hpp"


class
FileHandler
{
public:
    /// Fetches file path from cli arg
    /// \param arg_parse ArgParse class instance
    /// \return will return a path to a new file or given file from the input args
    static fs::path Fetch_File_Path(ArgParse *arg_parse);

    /// Saves file
    /// \param app_data AppData struct
    /// \return will return false on failure
    static bool Save_File(AppData *app_data);

    FileHandler() = delete;
};