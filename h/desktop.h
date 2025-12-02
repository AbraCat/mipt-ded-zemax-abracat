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
    Desktop(hui::UI* state, dr4::Vec2f size, cum::PPToolPlugin* pp_plugin);
    virtual ~Desktop();

    dr4::Texture* giveTexture() const;
    OptController* getOptController() const { return control; }
    std::vector<std::unique_ptr<pp::Tool>>& getTools() { return tools; }

    virtual void Redraw() const override;

private:
    // WContainer *button_cont;
    OptController* control;
    WContainer* tools_container;
    std::vector<std::unique_ptr<pp::Tool>> tools;
};

}

#endif // MY_APP_H