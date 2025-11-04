#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>


#include "dr4/window.hpp"
#include "dr4/texture.hpp"
#include "dr4/math/color.hpp"

#include "sdl-adapter.h"
#include "my-dr4-plugin.h"

dr4::DR4Backend* plugin = nullptr;
static dr4::Window* window = nullptr;
static dr4::Texture* texture = nullptr;

const double fps = 30;
const int begin_ticks = 1000, width = 1920, height = 1000;
bool first_frame = 1;

/*
virtual functions:
copying object
saving and reading scene configuration to/from file

multithreading segfault
better text rendering
3 views
propagate texture transformation
*/

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    srand(1);

    plugin = dr4::CreateDR4Backend();

    window = plugin->CreateWindow();
    window->Open();

    texture = window->CreateTexture();

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }

    if (event->type == SDL_EVENT_KEY_DOWN) return SDL_APP_SUCCESS;

    // if (event->type == SDL_EVENT_KEY_DOWN)
    // {
    //     if (event->key.key == SDLK_RSHIFT) return SDL_APP_SUCCESS;
    //     if (event->key.repeat == true) return SDL_APP_CONTINUE;

    //     if (state->focused != nullptr)
    //     {
    //         KeyboardEvent evt(event->key.key);
    //         state->focused->keyboardEvent(&evt);
    //     }

    //     return SDL_APP_CONTINUE;
    // }

    // if (event->type == SDL_EVENT_MOUSE_MOTION)
    // {
    //     MouseEvent e(MOUSE_MOVE, event->button.x, event->button.y);
    //     desktop->handleEvent(&e);
    //     return SDL_APP_CONTINUE;
    // }

    // if (event->button.button == SDL_BUTTON_LEFT)
    // {
    //     MouseEnum type;
    //     switch (event->type)
    //     {
    //         case SDL_EVENT_MOUSE_BUTTON_DOWN: type = MOUSE_DOWN; break;
    //         case SDL_EVENT_MOUSE_BUTTON_UP: type = MOUSE_UP; break;
    //         default: return SDL_APP_CONTINUE;
    //     }
        
    //     MouseEvent e(type, event->button.x, event->button.y);
    //     desktop->handleEvent(&e);
    //     return SDL_APP_CONTINUE;
    // }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    // IdleEvent evt;
    // desktop->handleEvent(&evt);

    // // desktop->t->renderRec();
    // desktop->t->renderIfUpdatedRec();
    // SDL_RenderPresent(renderer);

    // SDL_Delay(1000.0 / fps);

    dr4::Rectangle rect{dr4::Rect2f{dr4::Vec2f(100, 100), dr4::Vec2f(100, 100)}, dr4::Color(255, 0, 0, 255)};
    texture->Draw(rect);

    window->Display();
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    window->Close();
    delete window;
}
