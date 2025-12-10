#include "tool-widget.h"
#include "wcontainer.h"

#include <cassert>

namespace hui {

ToolWidget::ToolWidget(hui::UI *state, pp::Tool* tool, dr4::Vec2f pos, dr4::Vec2f size,
    dr4::Color color, std::string text, CanvasWidget* cvs_widget)
    : ExclusiveButton(state, pos, size, color, text), tool(tool), cvs_widget(cvs_widget)
{
    //
}

pp::Tool* ToolWidget::getTool() const { return tool; }

void ToolWidget::action() {
    ExclusiveButton::action();
    cvs_widget->setSelectedTool(tool);
    tool->OnStart();
}

void ToolWidget::deactivate() {
    tool->OnEnd();
    cvs_widget->setSelectedTool(nullptr); // TODO
}
    
} // namespace hui