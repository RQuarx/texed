#pragma once

#include "app_data.hpp"

#include <SDL3/SDL.h>


class
Decoration
{
public:
    static bool Draw_Decoration(AppData *AppData, Offset *offset);

    Decoration() = delete;
private:
    static bool Draw_File_Name(AppData *AppData, Offset *offset);
};