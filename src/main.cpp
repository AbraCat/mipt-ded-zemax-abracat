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

/*
TODO
refactor pp events
pp color selection (rgb)

wlist: padding
input field: cursor, +/- buttons
bounding box
main menu with list of plugins
ui refactor
*/

enum AppStatus {
    APP_CONTINUE,
    APP_SUCCESS,
    APP_ERROR
};

extern dr4::Window* window = nullptr;
extern const double ratio;
extern const int scene_w;

const int desktop_w = 1900, desktop_h = 1000, fps = 30;
const std::string dr4_path = "build/libdr4.so";
const std::string pp_path = "build/libpp.so";
const std::string font_path = "ttf/font.ttf";

double prev_hui_idle = -1, prev_pp_idle = -1;



hui::IdleEvent* getUiIdleEvent();
pp::IdleEvent getPpIdleEvent();
AppStatus processDr4Event(dr4::Event& evt, hui::UI* ui, std::vector<std::unique_ptr<pp::Tool>>& tools);
void ProcessToolsEvent(std::vector<std::unique_ptr<pp::Tool>>& tools, dr4::Event evt);

AppStatus init_app(hui::UI** ui, hui::Desktop** root_widget, dr4::Texture** main_texture);
AppStatus iterate_app(hui::UI* ui, dr4::Texture* main_texture,
    std::vector<std::unique_ptr<pp::Tool>>& tools);



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

AppStatus processDr4Event(dr4::Event& evt, hui::UI* ui, std::vector<std::unique_ptr<pp::Tool>>& tools) {
    if (evt.type == dr4::Event::Type::QUIT ||
        evt.type == dr4::Event::Type::KEY_DOWN && evt.key.sym == dr4::KeyCode::KEYCODE_ESCAPE)
    {
        window->Close();
        delete window;
        return APP_SUCCESS;
    }

    ProcessToolsEvent(tools, evt);
    ui->ProcessEvent(evt);
    return APP_CONTINUE;
}

void ProcessToolsEvent(std::vector<std::unique_ptr<pp::Tool>>& tools, dr4::Event evt) {
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
}

AppStatus init_app(hui::UI** ui, hui::Desktop** root_widget, dr4::Texture** main_texture) {
    cum::Manager* manager = new cum::Manager();
    assert(manager->LoadFromFile(dr4_path) != nullptr);
    assert(manager->LoadFromFile(pp_path) != nullptr);

    cum::DR4BackendPlugin* dr4_plugin = manager->GetAnyOfType<cum::DR4BackendPlugin>();
    assert(dr4_plugin != nullptr);

    window = dr4_plugin->CreateWindow();
    window->Open();
    *main_texture = window->CreateTexture();
    (*main_texture)->SetSize(desktop_w, desktop_h);

    dr4::Font* font = window->CreateFont();
    font->LoadFromFile(font_path);
    window->SetDefaultFont(font);

    cum::PPToolPlugin* pp_plugin = manager->GetAnyOfType<cum::PPToolPlugin>();
    assert(pp_plugin != nullptr);

    *ui = new hui::UI(window);
    *root_widget = new hui::Desktop(*ui, dr4::Vec2f(desktop_w, desktop_h), pp_plugin);
    (*ui)->SetRoot(*root_widget);
    return APP_CONTINUE;
}

AppStatus iterate_app(hui::UI* ui, dr4::Texture* main_texture,
  std::vector<std::unique_ptr<pp::Tool>>& tools) {
    hui::Widget* root_widget = ui->GetRoot();
    std::optional<dr4::Event> event;

    while ((event = window->PollEvent()).has_value())
    {
        AppStatus res = APP_CONTINUE;
        if ((res = processDr4Event(event.value(), ui, tools)) != APP_CONTINUE) return res;
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
    return APP_CONTINUE;
}

int main()
{
    srand(1);
    hui::UI* ui = nullptr;
    hui::Desktop* root_widget = nullptr;
    dr4::Texture* main_texture;
    if (init_app(&ui, &root_widget, &main_texture) == APP_ERROR) return 1;

    AppStatus iterate_res = APP_CONTINUE;
    while ((iterate_res = iterate_app(ui, main_texture, root_widget->getTools())) == APP_CONTINUE);
    if (iterate_res == APP_ERROR) return 1;

    delete main_texture;
    return 0;
}
