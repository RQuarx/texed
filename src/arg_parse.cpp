#include "../inc/arg_parse.hpp"

#include <SDL3/SDL.h>

#include <algorithm>


ArgParse::ArgParse(int argc, char **argv) {
    for (int i = 1; i < argc; i++)
        args.push_back(argv[i]);
}


bool
ArgParse::Arg(std::string arg, std::string long_arg)
{
    return (
        long_arg.empty() ?
        std::find(args.begin(), args.end(), arg) != args.end() :
        std::find(args.begin(), args.end(), arg) != args.end() ||
        std::find(args.begin(), args.end(), long_arg) != args.end()
    );
}


std::string
ArgParse::Arg_Option(std::string arg, std::string long_arg)
{
    auto it = std::find(this->args.begin(), this->args.end(), arg);

    if (it != this->args.end() && ++it != this->args.end())
        return *it;

    if (!long_arg.empty()) {
        it = std::find(this->args.begin(), this->args.end(), long_arg);
        if (it != this->args.end() && ++it != this->args.end())
            return *it;
    }

    return "";
}


std::optional<fs::path>
ArgParse::Get_File_Path()
{
    if (args.size() == 0) return nullopt;

    std::string arg_file = Arg_Option("-f", "--file");
    if (!arg_file.empty()) return fs::path(arg_file);

    if (
        fs::is_regular_file(args.back()) &&
        fs::exists(args.back())
    ) { return fs::path(args.back()); }

    return nullopt;
}


std::string
ArgParse::Get_File_Name()
{
    if (args.size() == 0) return "";

    std::string arg_file = Arg_Option("-f", "--file");
    if (!arg_file.empty()) return arg_file;

    if (
        args.back().front() != '-'
    ) { return args.back(); }

    return "";
}