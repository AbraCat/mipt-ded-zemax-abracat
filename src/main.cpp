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
        img->SetSize(dr4::Vec2f(1000, 800));
        for (int x = 100; x < 300; ++x) {
            for (int y = 300; y < 500; ++y)
            {
                if (((x / 10) % 2) ^ ((y / 10) % 2)) img->SetPixel(x, y, dr4::Color(255, 255, 0, 255));
                else img->SetPixel(x, y, dr4::Color(255, 0, 255, 255));
            }
        }

        texture->Draw(*img);

        dr4::Rectangle* rect = window->CreateRectangle();
        rect->SetSize(dr4::Vec2f(300, 100));
        rect->SetPos(100, 100);
        rect->SetFillColor(dr4::Color(0, 0, 255));
        rect->SetBorderColor(dr4::Color(255, 0, 0));

        texture->Draw(*rect);

        dr4::Text* text = window->CreateText();
        text->SetText("some text");
        text->SetPos(100, 100);
        text->SetColor(dr4::Color(0, 255, 0));
        texture->Draw(*text);

        window->Draw(*texture);
        window->Display();
    }

    return 0;
}
