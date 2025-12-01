#include "draggable.h"

#include "hui/ui.hpp"

#include <cassert>

DraggableWidget::DraggableWidget(hui::UI* ui, dr4::Vec2f pos, dr4::Vec2f size) : hui::Widget(ui) {
    SetPos(pos);
    SetSize(size);

    is_dragged = false;
    draggable = false;
}

void DraggableWidget::Redraw() const {
    dr4::Rectangle* rect = GetUI()->GetWindow()->CreateRectangle();
    rect->SetPos({0, 0});
    rect->SetSize(GetSize());
    rect->SetFillColor(dr4::Color(255, 0, 0));
    GetTexture().Draw(*rect);
}

void DraggableWidget::setDraggable(bool draggable) {
    this->draggable = draggable;

    assert(GetParent() != nullptr);
    drag_rect.pos = {0, 0};
    drag_rect.size = GetParent()->GetSize();
}

EventResult DraggableWidget::OnMouseDown(MouseButtonEvent &evt) {
    is_dragged = true;
    prev_widget_pos = GetPos();
    prev_event_pos = evt.pos;

    return hui::EventResult::HANDLED;
}

EventResult DraggableWidget::OnMouseUp(MouseButtonEvent &evt) {
    is_dragged = false;
    return hui::EventResult::HANDLED;
}

EventResult DraggableWidget::OnMouseMove(MouseMoveEvent &evt) {
    if (!is_dragged) return hui::EventResult::HANDLED;
    dr4::Vec2f new_pos = prev_widget_pos + (evt.pos - prev_event_pos);

    if (new_pos.x < drag_rect.pos.x) new_pos.x = drag_rect.pos.x;
    if (new_pos.x + GetSize().x > drag_rect.pos.x + drag_rect.size.x)
        new_pos.x = drag_rect.pos.x + drag_rect.size.x - GetSize().x;
    if (new_pos.y < drag_rect.pos.y) new_pos.y = drag_rect.pos.y;
    if (new_pos.y + GetSize().y > drag_rect.pos.y + drag_rect.size.y)
        new_pos.y = drag_rect.pos.y + drag_rect.size.y - GetSize().y;

    movePos(new_pos);
    ForceRedraw();

    prev_widget_pos = GetPos();
    prev_event_pos = evt.pos;
    return hui::EventResult::HANDLED;
}