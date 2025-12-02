#ifndef TOOL_WIDGET_H
#define TOOL_WIDGET_H

#include "pp/tool.hpp"
#include "button.h"

namespace hui {

class ToolWidget : public ExclusiveButton {
public:
    ToolWidget(hui::UI *state, pp::Tool* tool, dr4::Vec2f pos, dr4::Vec2f size,
            dr4::Color color, std::string text);
    virtual ~ToolWidget() noexcept override = default;

    pp::Tool* getTool() const;
    virtual void action() override;
    virtual void deactivate() override;

private:
    pp::Tool* tool;
};

}

#endif // TOOL_WIDGET_H