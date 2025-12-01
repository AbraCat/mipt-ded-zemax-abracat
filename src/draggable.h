#ifndef I_MY_DRAGGABLE_H
#define I_MY_DRAGGABLE_H

#include "hui/widget.hpp"

using hui::EventResult;
using hui::MouseButtonEvent;
using hui::MouseMoveEvent;

class DraggableWidget : public hui::Widget {
public:
    DraggableWidget(hui::UI* ui, dr4::Vec2f pos, dr4::Vec2f size);

    void setDraggable(bool draggable);
    void setDragRect(dr4::Rect2f rect) { drag_rect = rect; }

    virtual EventResult OnMouseDown(MouseButtonEvent &evt) override;
    virtual EventResult OnMouseUp(MouseButtonEvent &evt) override;
    virtual EventResult OnMouseMove(MouseMoveEvent &evt) override;
    virtual void Redraw() const override;

protected:
    bool draggable, is_dragged;
    dr4::Vec2f prev_widget_pos, prev_event_pos;
    dr4::Rect2f drag_rect;
};

#endif // I_MY_DRAGGABLE_H