#include "scroll.h"
#include "colors.h"

#include "hui/ui.hpp"

#include <cassert>
#include <cmath>

const double scroll_on_click = 0.1;

ScrollBar::ScrollBar(hui::UI* ui, dr4::Vec2f pos, dr4::Vec2f size, double init_frac) : MyContainer(ui, pos, size)
{
    width = size.x;
    height = size.y;
    assert(height > width * 3);
    setDrawRect(true);

    this->up_button = new ScrollButton(ui, {}, {width, width}, this, 1);
    this->down_button = new ScrollButton(ui, {0, height - width}, {width, width}, this, 0);
    addChild(up_button);
    addChild(down_button);

    this->thumb = new ScrollThumb(ui, {0, width}, {width, width}, this);
    addChild(thumb);
    thumb->setDraggable(true);
    thumb->setDragRect(dr4::Rect2f({0, width}, {width, height - width * 2}));
    frac_pos = 0;

    moveThumb(init_frac);
}

double ScrollBar::posToFrac(dr4::Vec2f thumbTL)
{
    int range = height - width * 3;
    return (thumbTL.y - width) * 1.0 / range;
}

dr4::Vec2f ScrollBar::fracToPos(double frac)
{
    int range = height - width * 3;
    return {0, frac * range + width};
}

void ScrollBar::moveThumb(double frac_change)
{
    frac_pos += frac_change;
    if (frac_pos < 0) frac_pos = 0;
    if (frac_pos > 1) frac_pos = 1;

    thumb->movePos(fracToPos(frac_pos));
}

void ScrollBar::thumbMoved(dr4::Vec2f newThumbTL)
{
    frac_pos = posToFrac(newThumbTL);
    action(frac_pos);
}

void ScrollBar::action(double frac_pos)
{
    //
}



ScrollButton::ScrollButton(hui::UI* ui, dr4::Vec2f pos, dr4::Vec2f size, ScrollBar* bar, bool up, dr4::Color color) : 
    Button(ui, pos, size, color, ""), up(up), bar(bar)
{
    //
}

void ScrollButton::action()
{
    bar->moveThumb(up ? -scroll_on_click : scroll_on_click);
}




ScrollThumb::ScrollThumb(hui::UI* ui, dr4::Vec2f pos, dr4::Vec2f size, ScrollBar* bar, dr4::Color color) :
    DraggableWidget(ui, pos, size), bar(bar), color(color)
{
    //
}

void ScrollThumb::movePos(dr4::Vec2f newTL)
{
    DraggableWidget::movePos(newTL);
    bar->thumbMoved(newTL);
}

void ScrollThumb::Redraw() const {
    dr4::Rectangle* rect = GetUI()->GetWindow()->CreateRectangle();
    rect->SetPos({0, 0});
    rect->SetSize(GetSize());
    rect->SetFillColor(color);
    rect->SetBorderColor(white_color);
    GetTexture().Draw(*rect);
}






WidgetScrollBar::WidgetScrollBar(hui::UI* ui, dr4::Vec2f pos, dr4::Vec2f size, ScrollableWidget* scrollable)
    : ScrollBar(ui, pos, size), scrollable(scrollable) {
    //
}

void WidgetScrollBar::action(double frac_pos) {
    scrollable->scroll(frac_pos);
}





ScrollableWidget::ScrollableWidget(hui::Widget* scrolled, dr4::Vec2f pos, dr4::Vec2f size, bool vertical)
    : MyContainer(scrolled->GetUI(), pos, size), scrolled(scrolled), vertical(vertical) {
    setDrawRect(true);
    addChild(scrolled);
}

void ScrollableWidget::scroll(double frac) {
    if (vertical) scrolled->SetPos({0, (GetSize().y - scrolled->GetSize().y) * frac});
    else scrolled->SetPos({(GetSize().x - scrolled->GetSize().x) * frac, 0});

    scrolled->ForceRedraw();
}


#if 0

MoveScrollBar::MoveScrollBar(Widget* parent, Vector tl, Vector br, Widget* w, 
    double amplitude, bool x_axis) : ScrollBar(parent, tl, br)
{
    this->w = w;
    this->x_axis = x_axis;
    this->amplitude = amplitude;

    this->init_centre = w->t->centre;
    this->init_scale_x = w->t->xScale;
}

int MoveScrollBar::fracToMovement(double frac)
{
    double scale_change = w->t->xScale / init_scale_x;
    Vector scaled_init_centre = w->wh * 0.5 + ((Vector)init_centre - w->wh * 0.5) * scale_change;

    int mid_pos = (x_axis ? scaled_init_centre.x : scaled_init_centre.y);
    int min_pos = mid_pos - amplitude * scale_change;
    int max_pos = mid_pos + amplitude * scale_change;

    return min_pos * (1 - frac) + max_pos * frac;
}

void MoveScrollBar::action(double frac)
{
    int movement = fracToMovement(frac);
    Vector move_vec;

    if (x_axis)
    {
        move_vec.x = movement;
        move_vec.y = w->t->centre.y;
    }
    else
    {
        move_vec.x = w->t->centre.x;
        move_vec.y = movement;
    }

    w->t->move(move_vec);
}

ScaleScrollBar::ScaleScrollBar(Widget* parent, Vector tl, Vector br, Widget* w, double scale_amplitude) :
    ScrollBar(parent, tl, br), w(w)
{
    this->scale_amplitude = scale_amplitude;
    init_scale_x = w->t->xScale;
    init_scale_y = w->t->yScale;
}

double ScaleScrollBar::fracToScale(double frac, bool x)
{
    double init_scale = (x ? init_scale_x : init_scale_y);
    double min_scale = init_scale / scale_amplitude;

    return min_scale * std::exp(std::log(scale_amplitude) * frac * 2);
}

double ScaleScrollBar::scaleToFrac(double scale, bool x)
{
    // f(scale / min_scale) = frac
    // f(1) = 0
    // f(scale_amp) = 0.5
    // f (scale_amp ^ 2) = 1
    // f(x) = log_{scale_amp^2}(x) = 0.5 * log_{scale_amp}(x)

    double init_scale = (x ? init_scale_x : init_scale_y);
    double min_scale = init_scale / scale_amplitude;

    return 0.5 * std::log(scale / min_scale) / std::log(scale_amplitude);
}

void ScaleScrollBar::action(double frac)
{
    w->t->rescaleCentre(fracToScale(frac, 1), fracToScale(frac, 0));
}

ListScrollBar::ListScrollBar(Widget* parent, Vector tl, Vector br, WList* list)
    : ScrollBar(parent, tl, br, 0), list(list)
{
    
}

void ListScrollBar::action(double frac) { list->scroll(frac); }

#endif // 0