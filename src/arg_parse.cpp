#include "../inc/arg_parse.hpp"

#include <SDL3/SDL.h>

#include <algorithm>
#include <print>


ArgParse::ArgParse(int argc, char **argv) {
    for (int i = 1; i < argc; i++)
        args.emplace_back(argv[i]);
}


bool
ArgParse::Arg(std::string_view arg, std::string_view long_arg)
{
    return (
        long_arg.empty() ?
        std::ranges::find(args, arg) != args.end():
        std::ranges::find(args, arg) != args.end() ||
        std::ranges::find(args, long_arg) != args.end()
    );
}


std::string
ArgParse::Arg_Option(std::string_view arg, std::string_view long_arg)
{
    auto it = std::ranges::find(this->args, arg);

    if (it != this->args.end() && it + 1 != this->args.end()) return *it;

    if (!long_arg.empty()) {
        it = std::ranges::find(this->args, long_arg);
        if (it != this->args.end() && it + 1 != this->args.end()) return *it;
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
        fs::exists(args.back()) &&
        fs::is_regular_file(args.back())
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


void
ArgParse::Print_Help_Msg()
{
    std::println("Usage: texed [options] [file]\n");
    std::println("Options:");
    std::println(" -h,--help                      show this message");
    std::println(" -v,--version                   show program version");
    std::println(" -V,--Verbose                   show logs");
    std::println(" -f,--file                      specify the path");
}