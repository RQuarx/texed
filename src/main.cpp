#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_hints.h>
#include <SDL3/SDL.h>

#include "../inc/input_handler.hpp"
#include "../inc/file_handler.hpp"
#include "../inc/decoration.hpp"
#include "../inc/arg_parse.hpp"
#include "../inc/app_data.hpp"
#include "../inc/editor.hpp"
#include "../inc/utils.hpp"

#include "../config.hpp"

#include <algorithm>

static const float ONE_SECOND_MS = 1000.0f;
static const char *APP_NAME = "texed";
static const char *APP_VERSION = "0.1.0";
static const char *APP_IDENTITY = "Simple Text Editor";


void
App_Quit(struct AppData *app_data)
{
    if (app_data->font) TTF_CloseFont(app_data->font);
    TTF_Quit();
    if (app_data->window) SDL_DestroyWindow(app_data->window);
    if (app_data->renderer) SDL_DestroyRenderer(app_data->renderer);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
};


SDL_AppResult
App_Event(struct AppData *app_data, SDL_Event *event)
{
    switch (event->type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;

    case SDL_EVENT_TEXT_INPUT:
        {
            auto editor_data = &app_data->editor_data;
            editor_data->file_content[editor_data->cursor.y]
                .insert(editor_data->cursor.x, event->text.text);
            editor_data->cursor.x += SDL_strlen(event->text.text);
        }
        app_data->changed = true;
        break;

    case SDL_EVENT_WINDOW_RESIZED:
        app_data->changed = true;
        break;

    case SDL_EVENT_WINDOW_MOUSE_ENTER:
        app_data->focused = true;
        app_data->changed = true;
        break;

    case SDL_EVENT_WINDOW_MOUSE_LEAVE:
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
        app_data->editor_data.scroll_offset = std::clamp(
            (int64_t)app_data->editor_data.scroll_offset -
            (int64_t)(event->wheel.y * scroll_multiplier),
            (int64_t)0,
            (int64_t)app_data->editor_data.file_content.size() - 1
        );
        app_data->changed = true;
        break;

    default:
        break;
    }
    return SDL_APP_CONTINUE;
}


SDL_AppResult
App_Iterate(struct AppData *app_data, Offset base_offset)
{
    if (!app_data->changed) return SDL_APP_CONTINUE;

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
        return SDL_APP_FAILURE;
    }

    if (!SDL_RenderClear(app_data->renderer)) {
        Log_Err("Failed to clear renderer");
        return SDL_APP_FAILURE;
    }

    if (!Decoration::Draw_Decoration(app_data, &base_offset))
        return SDL_APP_FAILURE;

    if (!Editor::Render_Loop(app_data, base_offset))
        return SDL_APP_FAILURE;

    if (!SDL_RenderPresent(app_data->renderer)) {
        Log_Err("Failed to update renderer");
        return SDL_APP_FAILURE;
    }

    app_data->changed = false;
    return SDL_APP_CONTINUE;
}


bool
Init_SDL(struct AppData *app_data)
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

    if (!app_data->font) {
        Log_Err("Failed to load font");
        return false;
    }

    if (app_data->verbose) Log_Debug("Creating window and renderer");

    SDL_CreateWindowAndRenderer(
        APP_NAME,
        800,
        600,
        SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE,
        &app_data->window,
        &app_data->renderer
    );

    if (!app_data->window) {
        Log_Err("Failed to create window");
        return false;
    }

    if (!app_data->renderer) {
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
    struct AppData app_data;

    /* Parsing CLI arguments */
    if (arg_parse.Arg("-h","--help")) {
        std::printf(HELP_MSG);
        return EXIT_SUCCESS;
    }

    if (arg_parse.Arg("-v", "--version")) {
        std::printf("%s-%s", APP_NAME, APP_VERSION);
        return EXIT_SUCCESS;
    }

    app_data.verbose = arg_parse.Arg("-V", "--verbose");

    /* Initialising SDL */
    if (!Init_SDL(&app_data)) {
        Log_Err("Failed to init SDL");
        App_Quit(&app_data);
        return EXIT_FAILURE;
    };

    if (app_data.verbose) Log_Debug("Creating editor & fetching file path");
    auto editor =
        Editor::Init_Editor(FileHandler::Fetch_File_Path(&arg_parse));

    if (!editor) {
        Log_Err("Failed to create editor, path is not a file");
        App_Quit(&app_data);
        return EXIT_FAILURE;
    }
    app_data.editor_data = *editor;

    if (app_data.verbose) Log_Debug("Fetching display mode");
    SDL_DisplayID *displays = SDL_GetDisplays(NULL);
    if (!displays) {
        Log_Err("Failed to get displays");
        App_Quit(&app_data);
        return EXIT_FAILURE;
    }

    const SDL_DisplayMode *display_mode = SDL_GetCurrentDisplayMode(*displays);
    if (!display_mode) {
        Log_Err("Failed to get display mode");
        App_Quit(&app_data);
        return EXIT_FAILURE;
    }

    app_data.changed = true;

    Log_Info("Initialisation complete");

    Offset offset = { 0, 0 };
    SDL_AppResult result;
    SDL_Event event;
    int32_t end; // Return code
    while (true) {
        SDL_Delay(ONE_SECOND_MS / display_mode->refresh_rate);

        while (SDL_PollEvent(&event)) result = App_Event(&app_data, &event);

        if (result != SDL_APP_CONTINUE) {
            end = (result == SDL_APP_SUCCESS ? EXIT_SUCCESS : EXIT_FAILURE);
            break;
        }

        result = App_Iterate(&app_data, offset);

        if (result != SDL_APP_CONTINUE) {
            end = (result == SDL_APP_SUCCESS ? EXIT_SUCCESS : EXIT_FAILURE);
            break;
        }
    }

    App_Quit(&app_data);
    return end;
}