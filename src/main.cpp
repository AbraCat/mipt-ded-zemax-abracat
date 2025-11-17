#include "dr4/window.hpp"
#include "dr4/texture.hpp"
#include "dr4/math/color.hpp"
#include "misc/dr4_ifc.hpp"

#include "optical-scene.h"

extern "C" dr4::DR4Backend* CreateDR4Backend(void);

extern dr4::Window* window = nullptr;
const int scene_w = 500;

int main()
{
    srand(1);
    dr4::DR4Backend* plugin = CreateDR4Backend();

    window = plugin->CreateWindow();
    window->Open();

    OptScene* scene = new OptScene(nullptr, nullptr, dr4::Vec2f(0, 0), dr4::Vec2f(scene_w, scene_w / ratio));
    scene->Redraw();

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

        window->Draw(*(scene->getTexture()));
        window->Display();

    }

    return 0;
}
