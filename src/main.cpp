#include "dr4/window.hpp"
#include "dr4/texture.hpp"
#include "dr4/math/color.hpp"
#include "misc/dr4_ifc.hpp"

extern "C" dr4::DR4Backend* CreateDR4Backend(void);

int main()
{
    srand(1);
    dr4::DR4Backend* plugin = CreateDR4Backend();

    dr4::Window* window = plugin->CreateWindow();
    window->Open();
    dr4::Texture* texture = window->CreateTexture();

    while (true)
    {
        std::optional<dr4::Event> event;
        while ((event = window->PollEvent()).has_value())
        {
            dr4::Event evt = event.value();

            if (evt.type == dr4::Event::Type::QUIT || evt.type == dr4::Event::Type::KEY_DOWN)
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
        text.text = "some text";
        text.pos = dr4::Vec2f(100, 100);
        text.color = dr4::Color(0, 255, 0, 255);
        texture->Draw(text);

        window->Draw(*texture, dr4::Vec2f(0, 0));
        window->Display();
    }

    return 0;
}
