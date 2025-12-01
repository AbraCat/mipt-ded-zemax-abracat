#ifndef W_CONTAINER_H
#define W_CONTAINER_H

#include "widget.h"

#include "hui/container.hpp"



class MyContainer : public hui::Container {
public:
    MyContainer(hui::UI* state, dr4::Vec2f pos = dr4::Vec2f(), dr4::Vec2f size = dr4::Vec2f());
    virtual ~MyContainer();
    void Redraw() const override;

    // void setDrawBorder(bool draw, dr4::Color color = dr4::Color(255, 255, 255));
    // void setFillRect(bool fill, dr4::Color color = dr4::Color(0, 0, 0));
    void setDrawRect(bool draw_rect, dr4::Color fill_col = dr4::Color(0, 0, 0), dr4::Color border_col = dr4::Color(255, 255, 255));

    virtual void clearChildren();
    virtual void addChild(Widget* w);
    virtual hui::EventResult PropagateToChildren(hui::Event &event) override;

    virtual int removeChildByPredicate(std::function<bool(Widget*)> predicate);

// protected:
    // bool draw_border, fill_rect;
    bool draw_rect;
    dr4::Color border_col, fill_col;
    std::vector<Widget*> children;
};



class WContainer : public MyContainer
{
public:
    WContainer(hui::UI* ui, dr4::Vec2f pos, dr4::Vec2f size, int nChildren, bool vertical);

    void resizeChild(int n_child);
    virtual void addChild(Widget* widget) override;
    dr4::Vec2f getChildSize() const { return dr4::Vec2f(childWidth, childHeight); }

    virtual int removeChildByPredicate(std::function<bool(Widget*)> predicate) override;

protected:
    bool vertical;
    int nChildren, padding, childWidth, childHeight;
};

class WList : public WContainer
{
public:
    WList(hui::UI* ui, dr4::Vec2f pos, bool vertical, double child_len, double edge_len);
    void resize();
    
    virtual void addChild(Widget* widget) override;
    virtual int removeChildByPredicate(std::function<bool(Widget*)> predicate) override;

private:
    double scroll_frac, child_len, edge_len;
};

#endif // W_CONTAINER_H