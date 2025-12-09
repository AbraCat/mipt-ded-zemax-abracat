#ifndef SCROLL_BAR_H
#define SCROLL_BAR_H

#include "wcontainer.h"
#include "button.h"
#include "draggable.h"

class ScrollThumb;
class ScrollButton;

class ScrollBar : public MyContainer
{
public:
    ScrollBar(hui::UI* ui, dr4::Vec2f pos, dr4::Vec2f size, double init_frac = 0.0);

    double posToFrac(dr4::Vec2f thumbPos);
    dr4::Vec2f fracToPos(double frac);

    void moveThumb(double frac_change);
    void thumbMoved(dr4::Vec2f newThumbPos);
    virtual void action(double frac_pos);

private:
    double frac_pos;
    ScrollButton *up_button, *down_button;
    ScrollThumb *thumb;
    double width, height;
};

class ScrollButton : public Button
{
public:
    ScrollButton(hui::UI* ui, dr4::Vec2f pos, dr4::Vec2f size, ScrollBar* bar, bool up, dr4::Color color = gray_color);
    virtual void action() override;

private:
    bool up;
    ScrollBar *bar;
};

class ScrollThumb : public DraggableWidget
{
public:
    ScrollThumb(hui::UI* ui, dr4::Vec2f pos, dr4::Vec2f size, ScrollBar* bar, dr4::Color color = dr4::Color(191, 191, 191));
    void movePos(dr4::Vec2f new_pos) override;
    void Redraw() const override;

private:
    ScrollBar *bar;
    dr4::Color color;
};




class ScrollableWidget : public MyContainer {
public:
    ScrollableWidget(hui::Widget* scrolled, dr4::Vec2f pos, dr4::Vec2f size, bool vertical = true);
    void scroll(double frac);

protected:
    Widget* scrolled;
    bool vertical;
};

class WidgetScrollBar : public ScrollBar {
public:
    WidgetScrollBar(hui::UI* ui, dr4::Vec2f pos, dr4::Vec2f size, ScrollableWidget* scrollable);
    virtual void action(double frac_pos) override;

protected:
    ScrollableWidget* scrollable;
};



// class MoveScrollBar : public ScrollBar
// {
// public:
//     MoveScrollBar(Widget* parent, Vector tl, Vector br, Widget* w, double amplitude, bool x_axis);

//     virtual void action(double frac) override;
//     int fracToMovement(double frac);

// private:
//     Widget *w;
//     bool x_axis;
//     double init_scale_x, amplitude;
//     Vector init_centre;
// };

// class ScaleScrollBar : public ScrollBar
// {
// public:
//     ScaleScrollBar(Widget* parent, Vector tl, Vector br, Widget* w, double scale_amplitude);
//     virtual void action(double frac) override;

//     double fracToScale(double frac, bool x);
//     double scaleToFrac(double scale, bool x);

// private:
//     Widget *w;
//     double init_scale_x, init_scale_y, scale_amplitude;
// };

// class ListScrollBar : public ScrollBar
// {
// public:
//     ListScrollBar(Widget* parent, Vector tl, Vector br, WList* list);
//     virtual void action(double frac) override;

// // private:
//     WList* list;
// };

#endif // SCROLL_BAR_H