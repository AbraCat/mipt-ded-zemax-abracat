#include "desktop.h"
#include "button.h"
#include "tool-widget.h"
#include "scroll.h"

#include "hui/ui.hpp"

#include <cmath>

const double src_size = 0.3;
extern const int opt_control_w, opt_control_h, tools_h, tool_button_w;

namespace hui {

Desktop::Desktop(hui::UI* state, dr4::Vec2f size, cum::PPToolPlugin* pp_plugin) :
    MyContainer(state)
{
    SetSize(size);

    control = new OptController(state, this);
    control->addSource({0, -1, 4}, green_col * 0.5, src_size);

    control->addSphere({-1, 0, 0}, gray_col, 0.5);
    control->addSphere({1, -0.5, 0}, gray_col, 0.5);
    control->addSphere({0.3, 1, 0}, gray_col, 0.5);
    // control->addSphere({0, 0, -12}, gray_col, 5);
    // control->addSphere({0, 0, -2}, purple_col, 0.3);
    // control->addSphere({0, 0, 3}, white_col, 1, glass);

    tools = pp_plugin->CreateTools(control->getCanvas());
    tools_container = new WContainer(GetUI(), {opt_control_w, 0}, {tool_button_w, tools_h}, tools.size(), 1);
    for (int n_tool = 0; n_tool < tools.size(); ++n_tool) {
        std::unique_ptr<pp::Tool>& tool = tools[n_tool];
        tools_container->addChild(new ToolWidget(GetUI(), &*tool, {}, {}, 
            dr4::Color(127, 127, 127), "Draw " + std::string(tool->Name())));
    }
    addChild(tools_container);
}

Desktop::~Desktop()
{
    // for (pp::Tool* tl: tools) delete tl;
}

dr4::Texture* Desktop::giveTexture() const { return &GetTexture(); }

void Desktop::Redraw() const
{
    // dr4::Rectangle* rect = GetUI()->GetWindow()->CreateRectangle();
    // rect->SetSize(GetSize());
    // GetTexture().Draw(*rect);

    MyContainer::Redraw();
}

// void Desktop::updateTexture()
// {
//     Widget::updateTexture();
// }

// bool Desktop::onIdle(IdleEvent* e)
// {
//     return 0;
// }

} // namepsace hui