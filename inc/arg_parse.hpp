#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "../inc/editor.hpp"

#define HELP_MSG \
    "Usage: texed [options] [file]\n\n" \
    "Options:\n" \
    " -h,--help                      show this message\n" \
    " -v,--version                   show program version\n" \
    " -V,--Verbose                   show logs\n" \
    " -f,--file                      specify the path\n"


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
    bool Arg(std::string arg, std::string long_arg = "");

    /// Returns the option given to an arg
    /// \param arg short arg form
    /// \param long_arg long arg form
    /// \return will return the option if found, else will return an empty string `""`
    std::string Arg_Option(std::string arg, std::string long_arg = "");

    /// Searches for an arg containing a file path
    /// \return returns the file path if found, else returns nullopt
    std::optional<fs::path> Get_File_Path();

    /// Searched for a file name (edited file)
    /// \return will return an empty string if none are found
    std::string Get_File_Name();
};