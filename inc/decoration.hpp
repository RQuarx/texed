#pragma once

#include "app_data.hpp"


class
Decoration
{
public:
    static bool Draw_Decoration(AppData *app_data, Offset *offset);

    Decoration() = delete;
private:
    static bool Draw_File_Name(AppData *app_data, Offset *offset);
};