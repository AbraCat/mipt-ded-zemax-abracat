#include "wcontainer.h"
#include "my-utils.h"

#include "hui/ui.hpp"

#include <cassert>

using namespace hui;

MyContainer::MyContainer(UI* state, dr4::Vec2f pos, dr4::Vec2f size) : Container(state) {
    SetSize(size);
    SetPos(pos);

    draw_rect = false;
} 

MyContainer::~MyContainer() {
    for (Widget* w: children) delete w;
}

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

    // if (draw_rect) {
    //     drawRectBorder(dr4::Rect2f({}, GetSize()), GetTexture(), GetUI()->GetWindow(), border_col);
    // }
}

void MyContainer::clearChildren() {
    children.clear();
    ForceRedraw();
}

void MyContainer::addChild(Widget* w) {
    children.push_back(w);
    Container::BecomeParentOf(w);
}


EventResult MyContainer::PropagateToChildren(hui::Event &event) {
    hui::KeyEvent* key_evt = dynamic_cast<hui::KeyEvent*>(&event);
    if (key_evt != nullptr) return EventResult::UNHANDLED;

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

    if (vertical) {
        w->SetPos(dr4::Vec2f(padding, padding * (nChild + 1) + childHeight * nChild));
        w->SetSize(dr4::Vec2f(GetSize().x - padding * 2, childHeight));
    }
    else {
        w->SetPos(dr4::Vec2f(padding * (nChild + 1) + childWidth * nChild, padding));
        w->SetSize(dr4::Vec2f(childWidth, GetSize().y - padding * 2));
    }

    w->ForceRedraw();
}

void WContainer::addChild(Widget* w)
{
    int nChild = children.size();
    MyContainer::addChild(w);
    resizeChild(nChild);

    //
}

int WContainer::removeChildByPredicate(std::function<bool(Widget*)> predicate)
{
    int n_removed = MyContainer::removeChildByPredicate(predicate);
    assert(n_removed != -1);

    for (int n_child = n_removed; n_child < children.size(); ++n_child)
        resizeChild(n_child);

    return n_removed;
}


GridContainer::GridContainer(hui::UI* ui, dr4::Vec2f pos, dr4::Vec2f size, int width, int height) 
    : WContainer(ui, pos, size, height, 1), width(width), height(height) {
    child_cnt = 0;
    for (int child_num = 0; child_num < height; ++child_num) {
        WContainer* cont = new WContainer(ui, {}, {size.x, size.y / height}, width, 0);
        cont->setDrawRect(true, black_color, black_color);
        addChild(cont);
    }
}

void GridContainer::addGridChild(Widget* widget) {
    assert(child_cnt < width * height);

    WContainer* cont = dynamic_cast<WContainer*>(children[child_cnt % width]);
    assert(cont != nullptr);

    cont->addChild(widget);
    widget->ForceRedraw();
    ++child_cnt;
}

void GridContainer::clearChildren() {
    child_cnt = 0;
    for (Widget* w: children) {
        WContainer* cont = dynamic_cast<WContainer*>(w);
        assert(cont != nullptr);
        cont->clearChildren();
    }

    ForceRedraw();
}




WList::WList(hui::UI* ui, dr4::Vec2f pos, bool vertical, double child_len, double edge_len)
    : WContainer(ui, pos, {edge_len, 0}, 0, vertical), child_len(child_len), edge_len(edge_len)
{
    // setFillRect(1);
    scroll_frac = 0;

    if (vertical)
    {
        childWidth = edge_len;
        childHeight = child_len;
    }
    else
    {
        childHeight = edge_len;
        childWidth = child_len;
    }
}

void WList::resize() {
    if (vertical) SetSize({edge_len, child_len * children.size()});
    else SetSize({child_len * children.size(), edge_len});
}

void WList::addChild(Widget* widget) {
    WContainer::addChild(widget);
    resize();
}

int WList::removeChildByPredicate(std::function<bool(Widget*)> predicate)
{
    int n_removed = WContainer::removeChildByPredicate(predicate);
    resize();
    return n_removed;
}