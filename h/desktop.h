#ifndef MY_APP_H
#define MY_APP_H

#include "button.h"
#include "optical-control.h"

#include "hui/widget.hpp"
#include "wcontainer.h"
#include "dr4/math/vec2.hpp"
#include "pp/tool.hpp"
#include "cum/ifc/pp.hpp"

namespace hui {

class Desktop : public MyContainer
{
    friend class OptController;
public:
    Desktop(hui::UI* ui, dr4::Vec2f size, cum::Manager* manager);
    virtual ~Desktop();

    OptController* getOptController() const { return control; }
    std::vector<std::unique_ptr<pp::Tool>>& getTools() { return tools; }

    void addPpColorField(WContainer* container, ColComponent component);
    WContainer* createPpColorContainer(dr4::Vec2f pos, dr4::Vec2f size);

    virtual void Redraw() const override;

private:
    // WContainer *button_cont;
    OptController* control;
    WContainer* tools_container;
    cum::Manager* manager;
    std::vector<std::unique_ptr<pp::Tool>> tools;
};

class MenuButton : public Button {
public:
    MenuButton(hui::UI *ui, dr4::Vec2f pos, dr4::Vec2f size, std::string text);
    virtual void action() override;
    virtual Widget* createWidget() = 0;
};

}

#endif // MY_APP_H