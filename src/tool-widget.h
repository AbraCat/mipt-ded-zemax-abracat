#ifndef TOOL_WIDGET_H
#define TOOL_WIDGET_H

#include "pp/tool.hpp"
#include "hui/widget.hpp"

namespace hui {

class ToolWidget : public Widget {
public:
    ToolWidget(pp::Tool* tool, const std::size_t width, const std::size_t height, Widget *parent);
    pp::Tool* getTool() const;

private:
    pp::Tool* tool;
};

}

#endif // TOOL_WIDGET_H