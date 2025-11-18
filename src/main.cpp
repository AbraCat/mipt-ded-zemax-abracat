#include "dr4/window.hpp"
#include "dr4/texture.hpp"
#include "dr4/math/color.hpp"
#include "cum/ifc/dr4.hpp"

#include "cum/ifc/pp.hpp"
#include "mycanvas.h"

#include "optical-scene.h"
#include "desktop.h"

extern "C" cum::DR4BackendPlugin* CreateDR4Backend(void);
extern "C" pp::PPToolPlugin* Create_PP_Plugin(void);

extern dr4::Window* window = nullptr;
const int desktop_w = 1900, desktop_h = 1000;

hui::Event* dr4ToHuiEvent(dr4::Event evt);

int iterate_app(hui::Widget* root_widget, dr4::Texture* main_texture, pp::MyCanvas* canvas,
  std::vector<pp::Tool*> tools) {
    std::optional<dr4::Event> event;
    while ((event = window->PollEvent()).has_value())
    {
        dr4::Event evt = event.value();

        if (evt.type == dr4::Event::Type::QUIT || evt.type == dr4::Event::Type::KEY_DOWN)
        {
            window->Close();
            delete window;
            return 1;
        }

        hui::Event* hui_event = dr4ToHuiEvent(evt);
        if (hui_event != nullptr) hui_event->Apply(*root_widget);

        for (pp::Tool* tl: tools) {
            if (evt.type == dr4::Event::Type::MOUSE_DOWN)
                tl->OnMouseDown(evt.mouseButton);
            else if (evt.type == dr4::Event::Type::MOUSE_UP)
                tl->OnMouseUp(evt.mouseButton);
            else if (evt.type == dr4::Event::Type::MOUSE_MOVE)
                tl->OnMouseMove(evt.mouseMove);
        }
    }

    root_widget->DrawOn(*main_texture);
    canvas->DrawAllShapes();

    window->Draw(*main_texture);
    window->Display();
    return 0;
}

int main()
{
    srand(1);
    cum::DR4BackendPlugin* dr4_plugin = CreateDR4Backend();
    hui::State* state = new hui::State();

    window = dr4_plugin->CreateWindow();
    window->Open();
    dr4::Texture* main_texture = window->CreateTexture();
    main_texture->SetSize(desktop_w, desktop_h);

    pp::MyCanvas* canvas = new pp::MyCanvas(window, dr4::Vec2f(desktop_w, desktop_h), main_texture);
    pp::PPToolPlugin* pp_plugin = Create_PP_Plugin();
    std::vector<pp::Tool*> tools = pp_plugin->CreateTools(canvas);

    hui::Desktop* root_widget = new hui::Desktop(state, dr4::Vec2f(desktop_w, desktop_h), tools);

    while (true)
    {
        if (iterate_app(root_widget, main_texture, canvas, tools)) return 0;
    }

    delete main_texture;
    delete root_widget;
    for (pp::Tool* tl: tools) delete tl;
    return 0;
}

hui::Event* dr4ToHuiEvent(dr4::Event evt) {
    switch (evt.type) {
        case dr4::Event::Type::MOUSE_DOWN: case dr4::Event::Type::MOUSE_UP:
        {
            hui::MouseButtonEvent* event = new hui::MouseButtonEvent();
            event->relPos = evt.mouseButton.pos;
            event->pressed = (evt.type == dr4::Event::Type::MOUSE_DOWN);
            return event;
        }
    }

    return nullptr;
}
