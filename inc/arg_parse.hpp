#pragma once

#include "../inc/editor.hpp"


class
ArgParse
{
public:
    ArgParse(int argc, char **argv);

    std::vector<std::string> args;

    /// Check whether theres the arg on the arg list
    /// \param arg short arg form
    /// \param long_arg long arg form
    /// \return will return true if arg or long_arg is detected, else false
    bool Arg(std::string_view arg, std::string_view long_arg = "");

    /// Returns the option given to an arg
    /// \param arg short arg form
    /// \param long_arg long arg form
    /// \return will return the option if found, else will return an empty string `""`
    std::string Arg_Option(std::string_view arg, std::string_view long_arg = "");

    /// Searches for an arg containing a file path
    /// \return returns the file path if found, else returns nullopt
    std::optional<fs::path> Get_File_Path();

    /// Searched for a file name (edited file)
    /// \return will return an empty string if none are found
    std::string Get_File_Name();

    static void Print_Help_Msg();
};