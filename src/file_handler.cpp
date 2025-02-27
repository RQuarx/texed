#include "../inc/file_handler.hpp"
#include "../inc/app_data.hpp"
#include "../inc/utils.hpp"
#include "../config.hpp"

#include <fstream>


fs::path
FileHandler::Fetch_File_Path(ArgParse *arg_parse)
{
    auto file_buff = arg_parse->Get_File_Path();

    if (file_buff == nullopt) {
        std::string file = arg_parse->Get_File_Name();
        if (file != "" && file.find('/') != std::string::npos)
            fs::create_directories(fs::path(file).parent_path());
        else file = new_file_name;

        std::ofstream txt_file(file);
        txt_file << new_file_text;
        txt_file.close();
        return fs::path(file);
    }

    if (fs::file_size(*file_buff) == 0) {
        std::ofstream ofile(*file_buff);
        ofile << new_file_text;
        ofile.close();
    }


    return *file_buff;
}


bool
FileHandler::Save_File(AppData *app_data)
{
    EditorData *editor_data = &app_data->editor_data;
    std::ofstream out_file(editor_data->file_name);
    std::ifstream in_file(editor_data->file_name);

    if (!out_file || !in_file) {
        Log_Err("Failed to save file: failed to get streams");
        return false;
    }

    std::string temp_str;
    for (size_t i = 0; i < editor_data->file_content.size(); i++) {
        std::string inserted_string = editor_data->file_content[i];
        if (i != editor_data->file_content.size() - 1) inserted_string += "\n";

        out_file << inserted_string;
    }
    return true;
}