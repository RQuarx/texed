#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL.h>

#include "../inc/input_handler.hpp"
#include "../inc/file_handler.hpp"
#include "../inc/decoration.hpp"
#include "../inc/arg_parse.hpp"
#include "../inc/app_data.hpp"
#include "../inc/editor.hpp"
#include "../inc/utils.hpp"
#include "log_utils.cpp"

#include "../config.hpp"

#include <algorithm>
#include <print>

static const float ONE_SECOND_MS = 1000.0F;
static const char *APP_NAME = "texed";
static const char *APP_VERSION = "0.2.1";
static const char *APP_IDENTITY = "Simple Text Editor";


void
App_Quit(AppData *app_data)
{
    for (auto & c : app_data->editor_data.cache) {
        SDL_DestroySurface(c.surface);
        SDL_DestroyTexture(c.texture);
    }

    if (app_data->font != nullptr) TTF_CloseFont(app_data->font);
    TTF_Quit();
    if (app_data->window != nullptr) SDL_DestroyWindow(app_data->window);
    if (app_data->renderer != nullptr) SDL_DestroyRenderer(app_data->renderer);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
};


bool
Handle_Scroll_Wheel(EditorData *editor_data, SDL_MouseWheelEvent *wheel)
{
    if ((SDL_GetModState() & SDL_KMOD_LSHIFT) != 0U) {
        auto max_elem = std::ranges::max_element(
            editor_data->file_content, StrLen_Compare
        );

        editor_data->scroll.x = std::clamp(
            editor_data->scroll.x -
            (int64_t)(wheel->y * scroll_multiplier),
            0L,
            (int64_t)max_elem->length()
        );
        return true;
    }

    if (wheel->y != 0){
        editor_data->scroll.y = std::clamp(
            editor_data->scroll.y -
            (int64_t)(wheel->y * scroll_multiplier),
            0L,
            (int64_t)editor_data->file_content.size() - 1
        );
        return true;
    }

    return false;
}


SDL_AppResult
App_Event(AppData *app_data, SDL_Event *event)
{
    switch (event->type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;

    case SDL_EVENT_TEXT_INPUT:
        {
            auto *editor_data = &app_data->editor_data;
            editor_data->file_content[editor_data->cursor.y]
                .insert(editor_data->cursor.x, event->text.text);
            editor_data->cursor.x += SDL_strlen(event->text.text);
        }
        app_data->changed = true;
        break;

    case SDL_EVENT_WINDOW_RESIZED:
        app_data->changed = true;
        break;

    case SDL_EVENT_WINDOW_FOCUS_GAINED:
        app_data->focused = true;
        app_data->changed = true;
        break;

    case SDL_EVENT_WINDOW_FOCUS_LOST:
        app_data->focused = false;
        app_data->changed = true;
        break;

    case SDL_EVENT_KEY_DOWN:
        app_data->changed = InputHandler::Handle(
            app_data,
            event->key.scancode
        );
        break;

    case SDL_EVENT_MOUSE_WHEEL:
        app_data->changed =
            Handle_Scroll_Wheel(&app_data->editor_data, &event->wheel);
        break;

    default:
        break;
    }
    return SDL_APP_CONTINUE;
}


bool
App_Iterate(AppData *app_data)
{
    if (!app_data->changed) return true;

    if (
        !SDL_SetRenderDrawColor(
            app_data->renderer,
            background.r,
            background.g,
            background.b,
            background.a
        )
    ) {
        Log_Err("Failed to set render color");
        return false;
    }

    if (!SDL_RenderClear(app_data->renderer)) {
        Log_Err("Failed to clear renderer");
        return false;
    }

    if (
        v_scrollbar_pos == -1 && show_decorations && show_v_scrollbar &&
        !Decoration::Draw_Vertical_ScrollBar(app_data, &app_data->editor_data.offset)
    ) return false;

    if (
        show_decorations && show_file_name &&
        !Decoration::Draw_File_Name(app_data, &app_data->editor_data.offset)
    ) return false;

    if (!Editor::Render_Loop(app_data)) return false;

    if (
        v_scrollbar_pos == 1 && show_decorations && show_v_scrollbar &&
        !Decoration::Draw_Vertical_ScrollBar(app_data, &app_data->editor_data.offset)
    ) return false;

    if (!SDL_RenderPresent(app_data->renderer)) {
        Log_Err("Failed to update renderer");
        return false;
    }

    app_data->changed = false;
    app_data->editor_data.offset.x = 0;
    app_data->editor_data.offset.y = 0;
    return true;
}


bool
Init_SDL(AppData *app_data)
{
    if (app_data->verbose) Log_Debug("Initialising video subsystem");
    if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        Log_Err("Video subsystem failed to init");
        return false;
    }

    if (app_data->verbose) Log_Debug("Setting app metadata");
    if (!SDL_SetAppMetadata(APP_NAME, APP_VERSION, APP_IDENTITY)) {
        Log_Err("Failed to set metadata");
        return false;
    }

    if (app_data->verbose) Log_Debug("Initialising SDL_TTF");
    if (!TTF_Init()) {
        Log_Err("Failed to init SDL_TTF");
        return false;
    }

    if (app_data->verbose) Log_Debug("Loading fonts");
    app_data->font = TTF_OpenFont(font_file, font_size);

    if (app_data->font == nullptr) {
        Log_Err("Failed to load font");
        return false;
    }

    if (app_data->verbose) Log_Debug("Creating window and renderer");

    if (
        !SDL_CreateWindowAndRenderer(
            APP_NAME,
            initial_window_width,
            initial_window_height,
            SDL_WINDOW_HIGH_PIXEL_DENSITY |
            (!window_always_floating ? SDL_WINDOW_RESIZABLE : 0),
            &app_data->window,
            &app_data->renderer
        )
    ) {
        Log_Err("Failed to create window and renderer");
        return false;
    }

    if (app_data->window == nullptr) {
        Log_Err("Failed to create window");
        return false;
    }

    if (app_data->renderer == nullptr) {
        Log_Err("Failed to create renderer");
        return false;
    }

    SDL_SetRenderVSync(app_data->renderer, 1);
    return true;
}


int32_t
main(int32_t argc, char **argv)
{
    ArgParse arg_parse(argc, argv);
    // AppData app_data;

    /* Parsing CLI arguments */
    if (arg_parse.Arg("-h","--help")) {
        ArgParse::Print_Help_Msg();
        return EXIT_SUCCESS;
    }

    if (arg_parse.Arg("-v", "--version")) {
        std::println("%s-%s", APP_NAME, APP_VERSION);
        return EXIT_SUCCESS;
    }

    bool verbose = arg_parse.Arg("-V", "--verbose");

    if (verbose) Log_Debug("Creating editor & fetching file path");
    fs::path file_path = FileHandler::Fetch_File_Path(&arg_parse);
    auto editor = Editor::Init_Editor(file_path);

    if (editor == nullptr) {
        Log_Err("Failed to create editor: {}", "path is invalid");
        return EXIT_FAILURE;
    }

    AppData app_data(&*editor);
    app_data.verbose = verbose;

    /* Initialising SDL */
    if (!Init_SDL(&app_data)) {
        Log_Err("Failed to init SDL");
        App_Quit(&app_data);
        return EXIT_FAILURE;
    };

    if (app_data.verbose) Log_Debug("Fetching display mode");
    SDL_DisplayID *displays = SDL_GetDisplays(nullptr);
    if (displays == nullptr) {
        Log_Err("Failed to get displays");
        App_Quit(&app_data);
        return EXIT_FAILURE;
    }

    const SDL_DisplayMode *display_mode = SDL_GetCurrentDisplayMode(*displays);
    if (display_mode == nullptr) {
        Log_Err("Failed to get display mode");
        App_Quit(&app_data);
        return EXIT_FAILURE;
    }

    app_data.changed = true;

    Log_Info("Initialisation complete");

    SDL_AppResult result;
    SDL_Event event;
    int32_t end; // Return code

    if (app_data.verbose) Log_Debug("Starting rendering process");
    while (true) {
        SDL_Delay(ONE_SECOND_MS / display_mode->refresh_rate);

        while (SDL_PollEvent(&event)) result = App_Event(&app_data, &event);

        if (result != SDL_APP_CONTINUE) {
            end = (result == SDL_APP_SUCCESS ? EXIT_SUCCESS : EXIT_FAILURE);
            break;
        }

        if (!App_Iterate(&app_data)) {
            end = EXIT_FAILURE;
            break;
        }
    }

    App_Quit(&app_data);
    return end;
}