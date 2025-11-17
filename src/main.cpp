#include "dr4/window.hpp"
#include "dr4/texture.hpp"
#include "dr4/math/color.hpp"
#include "cum/ifc/dr4.hpp"

#include "cum/ifc/pp.hpp"
#include "mycanvas.h"

#include "optical-scene.h"

extern "C" cum::DR4BackendPlugin* CreateDR4Backend(void);
extern "C" pp::PPToolPlugin* Create_PP_Plugin(void);

extern dr4::Window* window = nullptr;
const int scene_w = 1000;

int main()
{
    srand(1);
    cum::DR4BackendPlugin* dr4_plugin = CreateDR4Backend();

    window = dr4_plugin->CreateWindow();
    window->Open();
    dr4::Texture* main_texture = window->CreateTexture();
    main_texture->SetSize(scene_w, scene_w / ratio);

    OptScene* scene = new OptScene(nullptr, nullptr, dr4::Vec2f(0, 0), dr4::Vec2f(scene_w, scene_w / ratio));

    pp::MyCanvas* canvas = new pp::MyCanvas(window, dr4::Vec2f(scene_w, scene_w / ratio), main_texture);
    pp::PPToolPlugin* pp_plugin = Create_PP_Plugin();
    std::vector<pp::Tool*> tools = pp_plugin->CreateTools(canvas);
    tools[0]->OnStart();

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

            if (evt.type == dr4::Event::Type::MOUSE_DOWN)
                tools[0]->OnMouseDown(evt.mouseButton);
            else if (evt.type == dr4::Event::Type::MOUSE_UP)
                tools[0]->OnMouseUp(evt.mouseButton);
            else if (evt.type == dr4::Event::Type::MOUSE_MOVE)
                tools[0]->OnMouseMove(evt.mouseMove);
        }

        scene->DrawOn(*main_texture);
        canvas->DrawAllShapes();

        window->Draw(*main_texture);
        window->Display();

    }

    delete main_texture;
    delete scene;
    // for (pp::Tool* tl: tools) delete tl;
    return 0;
}
