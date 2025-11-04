#ifndef W_CONTAINER_H
#define W_CONTAINER_H

#include "widget.h"

#include "hui/container.hpp"

// namespace hui
// {
// class MyContainerWidget : public ContainerWidget
// {
// public:
//     MyContainerWidget(hui::State *state, dr4::Vec2f pos, dr4::Vec2f size);
//     ~MyContainerWidget();

//     virtual void Redraw() override;

//     virtual void addChild(hui::Widget* w);
//     virtual EventResult PropogateToChildren(Event &event) override;

//     std::vector<hui::Widget*> children;
// };
// }





class WContainer : public Widget
{
public:
    WContainer(Widget* parent, Vector tl, Vector br, int nChildren, bool vertical);

    void resizeChild(int n_child);
    virtual void addWidget(Widget* widget) override;

protected:
    bool vertical;
    int nChildren, padding, childWidth, childHeight;
};

class WList : public WContainer
{
public:
    WList(Widget* parent, Vector tl, Vector br, bool vertical, double child_len);

    virtual int removeChildByPredicate(std::function<bool(Widget*)> predicate) override;
    virtual Vector propagatedAbsTL() override;
    virtual bool handleEvent(Event* e) override;
    
    void scroll(double frac);

private:
    double scroll_frac, child_len;
};

#endif // W_CONTAINER_H