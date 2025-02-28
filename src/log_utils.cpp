#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>
#include <SDL3/SDL.h>

#include <print>

#define DEBUG_LABEL "\e[1;37m[\e[1;36mDEBUG\e[1;37m]:\e[0;0;0m"
#define INFO_LABEL "\e[1;37m[\e[1;32mINFO\e[1;37m]:\e[0;0;0m"
#define ERR_LABEL "\e[1;37m[\e[1;31mERROR\e[1;37m]:\e[0;0;0m"


template<typename... T>
void
Log_Err(fmt::format_string<T...> fmt, T&&... args)
{
    std::time_t time = std::time(nullptr);
    fmt::println(
        "{:%H:%M:%S} {} {}: {}",
        fmt::localtime(time),
        ERR_LABEL,
        fmt::format(fmt, args...),
        SDL_GetError()
    );
}


template<typename... T>
void
Log_Info(fmt::format_string<T...> fmt, T&&... args)
{
    std::time_t time = std::time(nullptr);
    fmt::println(
        "{:%H:%M:%S} {} {}",
        fmt::localtime(time),
        INFO_LABEL,
        fmt::format(fmt, args...)
    );
}


template<typename... T>
void
Log_Debug(fmt::format_string<T...> fmt, T&&... args)
{
    std::time_t time = std::time(nullptr);
    fmt::println(
        "{:%H:%M:%S} {} {}",
        fmt::localtime(time),
        DEBUG_LABEL,
        fmt::format(fmt, args...)
    );
}