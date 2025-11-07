#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>

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

int main()
{
    srand(1);
    plugin = dr4::CreateDR4Backend();

    window = plugin->CreateWindow();
    window->Open();

    texture = window->CreateTexture();

    while (true)
    {
        std::optional<dr4::Event> event;
        while ((event = window->PollEvent()).has_value())
        {
            dr4::Event evt = event.value();
            
            if (evt.type == dr4::Event::Type::QUIT)
            {
                window->Close();
                delete window;
                return 0;
            }
        }

        dr4::Image* img = window->CreateImage();
        for (int x = 100; x < 300; ++x) {
            for (int y = 300; y < 500; ++y)
            {
                if (((x / 10) % 2) ^ ((y / 10) % 2)) img->SetPixel(x, y, dr4::Color(255, 255, 0, 255));
                else img->SetPixel(x, y, dr4::Color(255, 0, 255, 255));
            }
        }
        texture->Draw(*img, dr4::Vec2f(0, 0));

        dr4::Rectangle rect{dr4::Rect2f{dr4::Vec2f(100, 100), dr4::Vec2f(300, 100)}, dr4::Color(0, 0, 255, 255)};
        texture->Draw(rect);

        dr4::Text text;
        text.text = "aboba";
        text.pos = dr4::Vec2f(100, 100);
        text.color = dr4::Color(0, 255, 0, 255);
        texture->Draw(text);

        window->Display();

        //
    }

    return 0;
}
