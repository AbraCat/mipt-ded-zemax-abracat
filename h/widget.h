#ifndef I_MY_WIDGET_H
#define I_MY_WIDGET_H

#include <vector>
#include <string>
#include <functional>

#include "myvector.h"
#include "event.h"

#include "hui/widget.hpp"
#include "dr4/texture.hpp"



class MyWidget : public hui::Widget {
public:
    MyWidget(hui::UI* ui, dr4::Vec2f pos = dr4::Vec2f(), dr4::Vec2f size = dr4::Vec2f());
    virtual void Redraw() const override;

    void setRoundedAngles(bool rounded_angles) { this->rounded_angles = rounded_angles; }

private:
    bool rounded_angles;
};












#if 0

class Widget;
class Event;
class MouseEvent;
class IdleEvent;
class KeyboardEvent;

/*
coordinate systems:

absTL - absolute
tl, br - parent
everything else - this (if not said otherwise)
*/

class Widget
{
public:
    Widget(Vector tl, Vector br, Widget* parent = nullptr);
    ~Widget();

    virtual void addWidget(Widget* child);
    virtual int removeChild(Widget* child);
    virtual int removeChildByPredicate(std::function<bool(Widget*)> predicate);
    void clearChildren();

    void setPixelTexture(bool pixel_texture);
    void setWidgetBorderVisible(bool visible);
    void setTextureBorderVisible(bool visible);
    void setFillRect(bool fill, Vector color = Vector(0, 0, 0));
    void setDraggable(Vector dragTL = Vector(), Vector dragBR = Vector());

    virtual Vector getAbsTL();
    virtual Vector propagatedAbsTL();
    void propagateAbsPos();

    void drawWidgetRect(bool fill, Vector color = Vector(0, 0, 0));
    bool inRect(Vector point);
    bool inAbsRect(Vector point);

    virtual void resize(Vector newtl, Vector newbr);
    virtual void movePos(Vector newtl);

    virtual void updateTexture();
    void updateTextureRec();

    virtual bool handleEvent(Event* e);
    virtual bool onIdle(IdleEvent* e);
    virtual bool mousePressEvent(MouseEvent* e);
    virtual bool mouseMoveEvent(MouseEvent* e);
    virtual bool mouseReleaseEvent(MouseEvent* e);
    virtual bool keyboardEvent(KeyboardEvent* e);
// protected:
    dr4::Texture *t;

    Vector tl, br, wh, absTL;
    int width, height;

    bool draggable, dragged;
    Vector dragTL, dragBR, dragMouse; // relative to parent

    Vector fill_rect_color;
    bool w_border_visible, t_border_visible, fill_rect, pixel_texture;

    Widget *parent;
    std::vector<Widget*> children;
};

#endif // 0




#endif // I_MY_WIDGET_H