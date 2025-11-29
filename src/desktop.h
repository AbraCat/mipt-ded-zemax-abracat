#ifndef MY_APP_H
#define MY_APP_H

#include "button.h"
#include "optical-control.h"

#include "hui/widget.hpp"
#include "wcontainer.h"
#include "dr4/math/vec2.hpp"
#include "pp/tool.hpp"

namespace hui {

class Desktop : public MyContainer
{
    friend class OptController;
public:
    Desktop(hui::UI* state, dr4::Vec2f size, std::vector<std::unique_ptr<pp::Tool>>& tools);
    virtual ~Desktop();
    dr4::Texture* giveTexture() const;

    virtual void Redraw() const override;

    // virtual void updateTexture() override;
    // virtual bool onIdle(IdleEvent* e) override;

private:
    // WContainer *button_cont;
    OptController* control;

    std::vector<std::unique_ptr<pp::Tool>>& tools;
};

}

#endif // MY_APP_H