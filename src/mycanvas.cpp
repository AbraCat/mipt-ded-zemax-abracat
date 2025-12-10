#include "mycanvas.h"
#include "colors.h"

#include "hui/ui.hpp"

#include <cassert>

static dr4::Event huiToDr4Event(const hui::Event& hui_evt) {
    dr4::Event dr4_evt;

    { const hui::MouseButtonEvent* mouse_but_evt = dynamic_cast<const hui::MouseButtonEvent*>(&hui_evt);
    if (mouse_but_evt != nullptr) {
        if (mouse_but_evt->pressed)
            dr4_evt.type = dr4::Event::Type::MOUSE_DOWN;
        else
            dr4_evt.type = dr4::Event::Type::MOUSE_UP;
        dr4_evt.mouseButton.pos = mouse_but_evt->pos;
        dr4_evt.mouseButton.button = mouse_but_evt->button;
        return dr4_evt;
    }}

    { const hui::MouseMoveEvent* mouse_move_evt = dynamic_cast<const hui::MouseMoveEvent*>(&hui_evt);
    if (mouse_move_evt != nullptr) {
        dr4_evt.type = dr4::Event::Type::MOUSE_MOVE;
        dr4_evt.mouseMove.pos = mouse_move_evt->pos;
        dr4_evt.mouseMove.rel = mouse_move_evt->rel;
        return dr4_evt;
    }}

    { const hui::KeyEvent* key_evt = dynamic_cast<const hui::KeyEvent*>(&hui_evt);
    if (key_evt != nullptr) {
        if (key_evt->pressed) dr4_evt.type = dr4::Event::Type::KEY_DOWN;
        else dr4_evt.type = dr4::Event::Type::KEY_UP;
        dr4_evt.key.sym = key_evt->key;
        dr4_evt.key.mods = key_evt->mods;
        return dr4_evt;
    }}

    dr4_evt.type = dr4::Event::Type::UNKNOWN;
    return dr4_evt;
}

namespace pp {

ControlsTheme Canvas::GetControlsTheme() const { return ControlsTheme(); }


MyCanvas::MyCanvas(dr4::Window* window, dr4::Vec2f size)
    : size(size), window(window), texture(nullptr) {

    theme.shapeFillColor = red_color;
}

ControlsTheme MyCanvas::GetControlsTheme() const { return theme; }

void MyCanvas::AddShape(Shape *shape) {
    shapes.push_back(shape);
}

void MyCanvas::DelShape(Shape *shape) {
    for (std::vector<Shape*>::iterator it = shapes.begin(); it != shapes.end(); ++it) {
        if (*it == shape) {
            shapes.erase(it);
            break;
        }
    }
    widget->ForceRedraw();
}

void MyCanvas::setWidget(hui::Widget* w) {
    this->widget = w;
    this->texture = &w->GetFreshTexture();
}

void MyCanvas::ShapeChanged(Shape *shape) {
    widget->ForceRedraw();
}

dr4::Window *MyCanvas::GetWindow() { return window; }

void MyCanvas::DrawAllShapes() {
    for (Shape* sh: shapes) {
        texture->Draw(*sh);
    }
}

void MyCanvas::SetSelectedShape(Shape *shape) {}
Shape *MyCanvas::GetSelectedShape() const { return nullptr; }

} // namespace pp




CanvasWidget::CanvasWidget(hui::UI* ui, dr4::Vec2f pos, Widget* w)
    : MyContainer(ui, pos, w->GetSize())
{
    cur_tool = nullptr;
    is_focused = false;

    this->w = w;
    this->cvs = new pp::MyCanvas(GetUI()->GetWindow(), w->GetSize());
    cvs->setWidget(this);
    addChild(w);
}

void CanvasWidget::Redraw() const {
    w->DrawOn(GetTexture());
    cvs->DrawAllShapes();
}


EventResult CanvasWidget::OnMouseDown(hui::MouseButtonEvent &evt) {
    // return hui::EventResult::UNHANDLED;
    if (!GetRect().Contains(evt.pos)) {
        if (is_focused) {
            GetUI()->ReportFocus(nullptr);
            is_focused = false;
        }
        return hui::EventResult::UNHANDLED;
    }
    GetUI()->ReportFocus(this);

    dr4::Event dr4_evt = huiToDr4Event(evt);
    if (cur_tool != nullptr) cur_tool->OnMouseDown(dr4_evt.mouseButton);
    for (pp::Shape* sh: cvs->shapes) {
        sh->OnMouseDown(dr4_evt.mouseButton);
    }

    return hui::EventResult::UNHANDLED;
}

EventResult CanvasWidget::OnMouseUp(hui::MouseButtonEvent &evt) {
    // return hui::EventResult::UNHANDLED;

    dr4::Event dr4_evt = huiToDr4Event(evt);
    if (cur_tool != nullptr) cur_tool->OnMouseUp(dr4_evt.mouseButton);
    for (pp::Shape* sh: cvs->shapes) {
        sh->OnMouseUp(dr4_evt.mouseButton);
    }

    return hui::EventResult::UNHANDLED;
}

EventResult CanvasWidget::OnMouseMove(hui::MouseMoveEvent &evt) {
    // return hui::EventResult::UNHANDLED;

    dr4::Event dr4_evt = huiToDr4Event(evt);
    if (cur_tool != nullptr) cur_tool->OnMouseMove(dr4_evt.mouseMove);
    for (pp::Shape* sh: cvs->shapes) {
        sh->OnMouseMove(dr4_evt.mouseMove);
    }

    return hui::EventResult::UNHANDLED;
}

EventResult CanvasWidget::OnKeyDown(hui::KeyEvent &evt) {
    // return hui::EventResult::UNHANDLED;

    if (evt.key == dr4::KeyCode::KEYCODE_Q) {
        if (cur_tool != nullptr) cur_tool->OnBreak();
        // ForceRedraw();
        return hui::EventResult::UNHANDLED;
    }

    dr4::Event dr4_evt = huiToDr4Event(evt);
    if (cur_tool != nullptr) cur_tool->OnKeyDown(dr4_evt.key);
    for (pp::Shape* sh: cvs->shapes) {
        sh->OnKeyDown(dr4_evt.key);
    }

    return hui::EventResult::UNHANDLED;
}

EventResult CanvasWidget::OnIdle(hui::IdleEvent &evt) {
    MyContainer::OnIdle(evt);
    pp::IdleEvent pp_evt;
    pp_evt.absTime = evt.absTime;
    pp_evt.deltaTime = evt.deltaTime;

    if (cur_tool != nullptr) cur_tool->OnIdle(pp_evt);
    for (pp::Shape* sh: cvs->shapes) {
        sh->OnIdle(pp_evt);
    }

    return hui::EventResult::UNHANDLED;
}
