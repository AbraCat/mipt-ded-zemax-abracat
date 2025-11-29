#include "wcontainer.h"

#include "hui/ui.hpp"

#include <cassert>

using namespace hui;

MyContainer::MyContainer(UI* state, dr4::Vec2f pos, dr4::Vec2f size) : Container(state) {
    SetSize(size);
    SetPos(pos);

    // draw_border = fill_rect = false;
    draw_rect = false;
} 

MyContainer::~MyContainer() {
    for (Widget* w: children) delete w;
}

// void MyContainer::setDrawBorder(bool draw, dr4::Color color) {
//     draw_border = draw;
//     border_col = color;
// }

// void MyContainer::setFillRect(bool fill, dr4::Color color) {
//     fill_rect = fill;
//     fill_col = color;
// }

void MyContainer::setDrawRect(bool draw_rect, dr4::Color fill_col, dr4::Color border_col) {
    this->draw_rect = draw_rect;
    this->fill_col = fill_col;
    this->border_col = border_col;
}

void MyContainer::Redraw() const {
    if (draw_rect) {
        dr4::Rectangle* rect = GetUI()->GetWindow()->CreateRectangle();
        rect->SetSize(GetSize());
        rect->SetFillColor(fill_col);
        rect->SetBorderColor(border_col);
        GetTexture().Draw(*rect);
    }


    for (Widget* w: children) {
        GetTexture().Draw(*w);
    }
}

void MyContainer::clearChildren() {
    children.clear();
}

void MyContainer::addChild(Widget* w) {
    children.push_back(w);
    Container::BecomeParentOf(w);
}

EventResult MyContainer::PropagateToChildren(hui::Event &event) {
    for (Widget* w: children) {
        if (event.Apply(*w) == EventResult::HANDLED) return EventResult::HANDLED;
    }
    return EventResult::UNHANDLED;
}

int MyContainer::removeChildByPredicate(std::function<bool(Widget*)> predicate)
{
    for (int n_child = 0; n_child < children.size(); ++n_child)
    {
        if (predicate(children[n_child]))
        {
            children.erase(children.begin() + n_child);
            return n_child;
        }
    }

    assert(0);
    return -1;
}



// hui::ContainerWidget::ContainerWidget(hui::State *state, dr4::Vec2f pos, dr4::Vec2f size)
//     : hui::Widget(state, pos, size)
// {
//     //
// }

// hui::MyContainerWidget::MyContainerWidget(hui::State *state, dr4::Vec2f pos, dr4::Vec2f size)
//     : hui::ContainerWidget(state, pos, size)
// {
//     //
// }

// hui::MyContainerWidget::~MyContainerWidget()
// {
//     for (hui::Widget* w: children)
//     {
//         delete w;
//     }
// }

// void hui::MyContainerWidget::Redraw()
// {
//     for (hui::Widget* w: children)
//     {
//         // printf("redrawing child\n");
//         w->DrawOn(*this->texture);
//     }
// }

// void hui::MyContainerWidget::addChild(hui::Widget* w)
// {
//     children.push_back(w);
//     w->SetParent(this);
// }

// hui::EventResult hui::MyContainerWidget::PropogateToChildren(Event &event)
// {
//     // TODO: change event coordinates

//     for (Widget* w: children)
//     {
//         EventResult res = hui::EventResult::UNHANDLED;
//         ContainerWidget* container = dynamic_cast<ContainerWidget*>(w);

//         if (container == nullptr) res = event.Apply(*w);
//         else res = container->PropogateToChildren(event);

//         if (res == EventResult::HANDLED) return res;
//     }

//     return event.Apply(*this);
// }




WContainer::WContainer(UI* ui, dr4::Vec2f pos, dr4::Vec2f size, int nChildren, bool vertical)
    : MyContainer(ui)
{
    SetSize(size);
    SetPos(pos);
    // setWidgetBorderVisible(1);
    this->vertical = vertical;
    this->nChildren = nChildren;
    this->padding = 0;
    setDrawRect(true);

    if (vertical)
    {
        this->childHeight = (size.y - padding * (nChildren + 1)) / nChildren;
        this->childWidth = size.x - 2 * padding;
    }
    else
    {
        this->childWidth = (size.x - padding * (nChildren + 1)) / nChildren;
        this->childHeight = size.y - 2 * padding;
    }
}

void WContainer::resizeChild(int nChild)
{
    Widget* w = children[nChild];

    // if (vertical) w->resize(Vector(padding, padding * (nChild + 1) + childHeight * nChild), 
    //     Vector(wh.x - padding, (padding + childHeight) * (nChild + 1)));

    // else w->resize(Vector(padding * (nChild + 1) + childWidth * nChild, padding), 
    //     Vector((padding + childWidth) * (nChild + 1), wh.y - padding));

    if (vertical) {
        // w->resize(Vector(padding, padding * (nChild + 1) + childHeight * nChild), 
        // Vector(wh.x - padding, (padding + childHeight) * (nChild + 1)));

        w->SetPos(dr4::Vec2f(padding, padding * (nChild + 1) + childHeight * nChild));
        w->SetSize(dr4::Vec2f(GetSize().x - padding * 2, childHeight));
    }
    else {
        // w->resize(Vector(padding * (nChild + 1) + childWidth * nChild, padding), 
        // Vector((padding + childWidth) * (nChild + 1), wh.y - padding));

        w->SetPos(dr4::Vec2f(padding * (nChild + 1) + childWidth * nChild, padding));
        w->SetSize(dr4::Vec2f(childWidth, GetSize().y - padding * 2));
    }

    w->ForceRedraw();
}

void WContainer::addChild(Widget* w)
{
    // assert(children.size() < nChildren);
    int nChild = children.size();
    MyContainer::addChild(w);

    // w->t->setVisibleIn(this);
    resizeChild(nChild);   
    // w->ForceRedraw(); 
}

int WContainer::removeChildByPredicate(std::function<bool(Widget*)> predicate)
{
    int n_removed = MyContainer::removeChildByPredicate(predicate);
    assert(n_removed != -1);

    for (int n_child = n_removed; n_child < children.size(); ++n_child)
        resizeChild(n_child);

    return n_removed;
}




// WList::WList(Widget* parent, Vector tl, Vector br, bool vertical, double child_len)
//     : WContainer(parent, tl, br, 1, vertical), child_len(child_len)
// {
//     setFillRect(1);
//     scroll_frac = 0;

//     if (vertical)
//     {
//         childWidth = width;
//         childHeight = child_len;
//     }
//     else
//     {
//         childHeight = height;
//         childWidth = child_len;
//     }
// }

// int WList::removeChildByPredicate(std::function<bool(Widget*)> predicate)
// {
//     int n_removed = Widget::removeChildByPredicate(predicate);

//     for (int n_child = n_removed; n_child < children.size(); ++n_child)
//         resizeChild(n_child);

//     updateTextureRec();
//     return n_removed;
// }

// Vector WList::propagatedAbsTL()
// {
//     if (vertical)
//         return Vector(absTL.x, absTL.y - scroll_frac * (child_len * children.size() - height));
//     else
//         return Vector(absTL.x - scroll_frac * (child_len * children.size() - width), absTL.y);
// }

// void WList::scroll(double frac)
// {
//     scroll_frac = frac;
//     propagateAbsPos();
//     t->updated = 1;
// }

// bool WList::handleEvent(Event* e)
// {
//     MouseEvent* press_evt = dynamic_cast<MouseEvent*>(e);
//     if (press_evt != NULL && press_evt->type == MOUSE_DOWN &&
//         !inAbsRect(Vector(press_evt->x, press_evt->y)))
//     {
//         return e->dispatch(this);
//     }

//     for (Widget* w: children)
//         if (w->handleEvent(e)) return 1;
//     return e->dispatch(this);
// }
