#ifndef MY_APP_H
#define MY_APP_H

#include "widget.h"
#include "button.h"
// #include "optical-control.h"
#include "optical-scene.h"

#include "hui/widget.hpp"
#include "wcontainer.h"
#include "dr4/math/vec2.hpp"
#include "pp/tool.hpp"

namespace hui {

class Desktop : public MyContainer
{
public:
    Desktop(hui::State* state, dr4::Vec2f size, pp::Tool* tool);
    ~Desktop();
    dr4::Texture* giveTexture() const;

    // virtual void Redraw() override;

    // virtual void updateTexture() override;
    // virtual bool onIdle(IdleEvent* e) override;

private:
    // WContainer *button_cont;
    // OptController* control;
    pp::Tool* tool;
};

}

#endif // MY_APP_H