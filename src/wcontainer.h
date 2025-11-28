#ifndef W_CONTAINER_H
#define W_CONTAINER_H

#include "widget.h"

#include "hui/container.hpp"

namespace hui
{

class MyContainer : public Container {
public:
    MyContainer(UI* state);
    virtual ~MyContainer();
    void Redraw() const override;

    virtual void addChild(Widget* w);
    virtual EventResult PropagateToChildren(Event &event) override;

protected:
    std::vector<Widget*> children;
};



class WContainer : public MyContainer
{
public:
    WContainer(UI* ui, dr4::Vec2f pos, dr4::Vec2f size, int nChildren, bool vertical);

    void resizeChild(int n_child);
    virtual void addChild(Widget* widget) override;

protected:
    bool vertical;
    int nChildren, padding, childWidth, childHeight;
};

// class WList : public WContainer
// {
// public:
//     WList(Widget* parent, Vector tl, Vector br, bool vertical, double child_len);

//     virtual int removeChildByPredicate(std::function<bool(Widget*)> predicate) override;
//     virtual Vector propagatedAbsTL() override;
//     virtual bool handleEvent(Event* e) override;
    
//     void scroll(double frac);

// private:
//     double scroll_frac, child_len;
// };

} // namespace hui

#endif // W_CONTAINER_H