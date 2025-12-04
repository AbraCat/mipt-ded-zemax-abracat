#include "dr4/window.hpp"
#include "dr4/texture.hpp"
#include "dr4/math/color.hpp"
#include "hui/ui.hpp"
#include "cum/ifc/dr4.hpp"
#include "cum/manager.hpp"
#include "cum/ifc/pp.hpp"

#include "mycanvas.h"
#include "optical-scene.h"
#include "desktop.h"

#include <cassert>
#include <thread>
#include <chrono>
#include <iostream>

extern dr4::Window* window = nullptr;
extern const double ratio;
extern const int scene_w;

const int desktop_w = 1900, desktop_h = 1000, fps = 30;
const std::string dr4_path = "build/libdr4.so";
const std::string pp_path = "build/libpp.so";
const std::string font_path = "ttf/font.ttf";

double prev_hui_idle = -1, prev_pp_idle = -1;

int iterate_app(hui::UI* ui, dr4::Texture* main_texture,
    std::vector<std::unique_ptr<pp::Tool>>& tools);
hui::IdleEvent* getUiIdleEvent();
pp::IdleEvent getPpIdleEvent();

hui::IdleEvent* getUiIdleEvent() {
    hui::IdleEvent* idle_evt = new hui::IdleEvent();

    idle_evt->absTime = window->GetTime();
    if (prev_hui_idle == -1) idle_evt->deltaTime = 0;
    else idle_evt->deltaTime = idle_evt->absTime - prev_hui_idle;

    prev_hui_idle = idle_evt->absTime;
    return idle_evt;
}

pp::IdleEvent getPpIdleEvent() {
    pp::IdleEvent idle_evt;

    idle_evt.absTime = window->GetTime();
    idle_evt.absTime = window->GetTime();
    if (prev_pp_idle == -1) idle_evt.deltaTime = 0;
    else idle_evt.deltaTime = idle_evt.absTime - prev_pp_idle;

    prev_pp_idle = idle_evt.absTime;
    return idle_evt;
}

int iterate_app(hui::UI* ui, dr4::Texture* main_texture,
  std::vector<std::unique_ptr<pp::Tool>>& tools) {
    hui::Widget* root_widget = ui->GetRoot();
    std::optional<dr4::Event> event;

    while ((event = window->PollEvent()).has_value())
    {
        dr4::Event evt = event.value();

        if (evt.type == dr4::Event::Type::QUIT ||
            evt.type == dr4::Event::Type::KEY_DOWN && evt.key.sym == dr4::KeyCode::KEYCODE_ESCAPE)
        {
            window->Close();
            delete window;
            return 1;
        }

        for (std::unique_ptr<pp::Tool>& tl: tools) {
            switch (evt.type) {
                case dr4::Event::Type::MOUSE_DOWN:
                    tl->OnMouseDown(evt.mouseButton);
                    break;
                case dr4::Event::Type::MOUSE_UP:
                    tl->OnMouseUp(evt.mouseButton);
                    break;
                case dr4::Event::Type::MOUSE_MOVE:
                    tl->OnMouseMove(evt.mouseMove);
                    break;
                case dr4::Event::Type::KEY_DOWN:
                    tl->OnKeyDown(evt.key);
                    break;
            }
        }

        ui->ProcessEvent(evt);
    }
    
    hui::IdleEvent* hui_idle_evt = getUiIdleEvent();
    ui->OnIdle(*hui_idle_evt);
    delete hui_idle_evt;

    pp::IdleEvent pp_idle_evt = getPpIdleEvent();
    for (std::unique_ptr<pp::Tool>& tl: tools) tl->OnIdle(pp_idle_evt);

    main_texture->Draw(*ui->GetRoot());
    window->Draw(*main_texture);
    window->Display();

    std::this_thread::sleep_for(std::chrono::milliseconds(1000 / fps));
    return 0;
}

int main()
{
    srand(1);
    cum::Manager* manager = new cum::Manager();
    assert(manager->LoadFromFile(dr4_path) != nullptr);
    assert(manager->LoadFromFile(pp_path) != nullptr);

    cum::DR4BackendPlugin* dr4_plugin = manager->GetAnyOfType<cum::DR4BackendPlugin>();
    assert(dr4_plugin != nullptr);

    window = dr4_plugin->CreateWindow();
    window->Open();
    dr4::Texture* main_texture = window->CreateTexture();
    main_texture->SetSize(desktop_w, desktop_h);

    dr4::Font* font = window->CreateFont();
    font->LoadFromFile(font_path);
    window->SetDefaultFont(font);

    cum::PPToolPlugin* pp_plugin = manager->GetAnyOfType<cum::PPToolPlugin>();
    assert(pp_plugin != nullptr);

    hui::UI* state = new hui::UI(window);
    hui::Desktop* root_widget = new hui::Desktop(state, dr4::Vec2f(desktop_w, desktop_h), pp_plugin);
    state->SetRoot(root_widget);

    while (true)
    {
        if (iterate_app(state, main_texture, root_widget->getTools())) return 0;
    }

    delete main_texture;
    return 0;
}
