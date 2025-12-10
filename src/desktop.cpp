#include "desktop.h"
#include "button.h"
#include "tool-widget.h"
#include "scroll.h"
#include "colors.h"

#include "hui/ui.hpp"
#include "cum/manager.hpp"

#include <cmath>
#include <cassert>

const double src_size = 0.3;
const int menu_h = 50, menu_w = 500;
extern const int opt_control_w, opt_control_h, tools_h, tool_button_w;

namespace hui {

Desktop::Desktop(hui::UI* state, dr4::Vec2f size, cum::Manager* manager) :
    MyContainer(state), manager(manager)
{
    SetSize(size);

    auto& plugins = manager->GetAll();
    WContainer* plugins_cont = new WContainer(state, {0, 0}, {menu_w, menu_h}, plugins.size(), 0);
    for (auto& plugin: plugins) {
        TextField* field = new TextField(state, {}, {});
        field->SetText(std::string(plugin->GetName()));
        plugins_cont->addChild(field);
    }
    addChild(plugins_cont);

    control = new OptController(state, this, {0, menu_h});
    control->addSource({0, -1, 4}, green_col * 0.5, src_size);

    control->addSphere({-1, 0, 0}, gray_col, 0.5);
    control->addSphere({1, -0.5, 0}, gray_col, 0.5);
    control->addSphere({0.3, 1, 0}, gray_col, 0.5);
    // control->addSphere({0, 0, -12}, gray_col, 5);
    // control->addSphere({0, 0, -2}, purple_col, 0.3);
    // control->addSphere({0, 0, 3}, white_col, 1, glass);

    cum::PPToolPlugin* pp_plugin = manager->GetAnyOfType<cum::PPToolPlugin>();
    assert(pp_plugin != nullptr);

    tools = pp_plugin->CreateTools(control->getCanvas());
    tools_container = new WContainer(GetUI(), {1400 - tool_button_w, menu_h}, {tool_button_w, tools_h}, tools.size(), 1);
    for (int n_tool = 0; n_tool < tools.size(); ++n_tool) {
        std::unique_ptr<pp::Tool>& tool = tools[n_tool];
        tools_container->addChild(new ToolWidget(GetUI(), &*tool, {}, {}, 
            gray_color, "Draw " + std::string(tool->Name()), control->scene_cvs_widget));
    }
    addChild(tools_container);
}

Desktop::~Desktop()
{
    //
}

void Desktop::Redraw() const
{
    MyContainer::Redraw();
}

void Desktop::addPpColorField(WContainer* container, ColComponent component) {
    // TextField* field = new TextField(GetUI(), )
}

WContainer* Desktop::createPpColorContainer(dr4::Vec2f pos, dr4::Vec2f size) {
    // GridContainer* cont = new GridContainer(GetUI(), pos, size, 2, 3);
    return nullptr;
}

} // namepsace hui