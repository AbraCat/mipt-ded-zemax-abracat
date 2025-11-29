#include "desktop.h"
#include "button.h"
#include "tool-widget.h"

#include "hui/ui.hpp"

#include <cmath>

const double src_size = 0.3;
extern const int opt_control_w, opt_control_h;
const int button_w = 200;

namespace hui {

Desktop::Desktop(hui::UI* state, dr4::Vec2f size, std::vector<std::unique_ptr<pp::Tool>>& tools) :
    MyContainer(state), tools(tools)
{
    // setFillRect(1);
    SetSize(size);

    control = new OptController(state, this);
    control->addSource({0, -1, 4}, green_col * 0.5, src_size);

    control->addSphere({-1, 0, 0}, gray_col, 0.5);
    control->addSphere({1, -0.5, 0}, gray_col, 0.5);
    control->addSphere({0.3, 1, 0}, gray_col, 0.5);
    control->addSphere({0, 0, -12}, gray_col, 5);
    control->addSphere({0, 0, -2}, purple_col, 0.3);
    control->addSphere({0, 0, 3}, white_col, 1, glass);

    const int tool_but_h = opt_control_h / tools.size();
    for (int n_tool = 0; n_tool < tools.size(); ++n_tool) {
        std::unique_ptr<pp::Tool>& tool = tools[n_tool];

        Button* tool_button = new ToolWidget(state, &*tool, 
            dr4::Vec2f(opt_control_w, tool_but_h * n_tool), dr4::Vec2f(button_w, tool_but_h),
            dr4::Color(255, 0, 0), std::string(tool->Name()));

        addChild(tool_button);
    }
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