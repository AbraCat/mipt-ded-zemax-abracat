#include "hui/event.hpp"
#include "hui/widget.hpp"

#include "dr4/event.hpp"

namespace hui {

EventResult MouseButtonEvent::Apply(Widget &widget) {
    if (pressed)
        return widget.OnMouseDown(*this);
    else
        return widget.OnMouseUp(*this);
}

EventResult MouseMoveEvent::Apply(Widget &widget) {
    return widget.OnMouseMove(*this);
}

EventResult MouseWheelEvent::Apply(Widget &widget) {
    return widget.OnMouseWheel(*this);
}

EventResult KeyEvent::Apply(Widget &widget) {
    if (pressed)
        return widget.OnKeyDown(*this);
    else
        return widget.OnKeyUp(*this);
}

EventResult TextEvent::Apply(Widget &widget) {
    return widget.OnText(*this);
}

EventResult IdleEvent::Apply(Widget &widget) {
    widget.OnIdle(*this);
    return EventResult::HANDLED;
}

// hui::Event* dr4ToHuiEvent(dr4::Event evt) {
//     switch (evt.type) {
//         case dr4::Event::Type::MOUSE_DOWN: case dr4::Event::Type::MOUSE_UP:
//         {
//             hui::MouseButtonEvent* event = new hui::MouseButtonEvent();
//             event->relPos = evt.mouseButton.pos;
//             event->pressed = (evt.type == dr4::Event::Type::MOUSE_DOWN);
//             return event;
//         }
//     }

//     return nullptr;
// }

};