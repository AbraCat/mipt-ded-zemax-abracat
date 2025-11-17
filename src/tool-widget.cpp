#include "tool-widget.h"

namespace hui {

ToolWidget::ToolWidget(hui::State *state, pp::Tool* tool, dr4::Vec2f pos, dr4::Vec2f size,
    dr4::Color color, std::string text)
    : ToggleButton(state, pos, size, color, text), tool(tool)
{
    //
}
pp::Tool* ToolWidget::getTool() const { return tool; }
void ToolWidget::action() { tool->OnStart(); }
void ToolWidget::deactivate() { tool->OnEnd(); }
    
} // namespace hui