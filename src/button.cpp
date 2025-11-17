#include "button.h"
#include "sdl-adapter.h"

#include "dr4/window.hpp"

#include <cassert>

const double unpressColorCoeff = 0.7;
const int key_enter = 13;

extern dr4::Window* window;

namespace hui {

TextField::TextField(hui::State *state, dr4::Vec2f pos, dr4::Vec2f size)
    : Widget(state)//, color(color), text(text)
{
    SetPos(pos);
    SetSize(size);
    color = dr4::Color(0, 0, 0);
    draw_border = true;
}

void TextField::Redraw() const
{
    const int text_h = 10;

    dr4::Rectangle* rect = window->CreateRectangle();
    rect->SetSize(GetSize());
    rect->SetBorderColor(dr4::Color(255, 255, 255));
    rect->SetFillColor(color);
    GetTexture().Draw(*rect);

    dr4::Text* text_drawable = window->CreateText();
    text_drawable->SetText(text);
    text_drawable->SetColor(dr4::Color(255, 255, 255));
    text_drawable->SetPos(dr4::Vec2f(0, GetSize().y / 2 - text_h));
    GetTexture().Draw(*text_drawable);
}

void TextField::SetFieldColor(dr4::Color color) { this->color = color; }//Redraw(); }
void TextField::SetText(std::string text) { this->text = text; }//Redraw(); }
std::string TextField::getText() { return text; }




Button::Button(hui::State *state, dr4::Vec2f pos, dr4::Vec2f size, dr4::Color color, std::string text)
    : TextField(state, pos, size)
{
    this->press_color = color;
    this->unpress_color = dr4::Color(press_color.r * unpressColorCoeff, 
                                     press_color.g * unpressColorCoeff, 
                                      press_color.b * unpressColorCoeff);
    this->is_pressed = false;
    this->text = text;

    SetText(text);
    SetFieldColor(unpress_color);
}

EventResult Button::OnMouseDown(MouseButtonEvent &evt)
{
    // Widget::mousePressEvent(e);

    if (!GetRect().Contains(evt.relPos)) return EventResult::UNHANDLED;
    if (is_pressed) return EventResult::UNHANDLED;

    is_pressed = 1;
    SetFieldColor(press_color);
    ForceRedraw();
    action();
    return EventResult::HANDLED;
}

EventResult Button::OnMouseUp(MouseButtonEvent &evt)
{
    // Widget::mouseReleaseEvent(e);

    is_pressed = false;
    SetFieldColor(unpress_color);
    ForceRedraw();
    return EventResult::UNHANDLED;
}

void Button::unpress()
{
    is_pressed = 0;
}





ToggleButton::ToggleButton(hui::State *state, dr4::Vec2f pos, dr4::Vec2f size, 
    dr4::Color color, std::string text)
    : Button(state, pos, size, color, text)
{
    //
}

EventResult ToggleButton::OnMouseDown(MouseButtonEvent &evt)
{
    // Widget::mousePressEvent(e);

    if (!GetRect().Contains(evt.relPos)) return EventResult::UNHANDLED;

    is_pressed = !is_pressed;
    SetFieldColor(is_pressed ? press_color : unpress_color);
    ForceRedraw();

    if (is_pressed) action();
    else deactivate();
    return EventResult::UNHANDLED;
}

EventResult ToggleButton::OnMouseUp(MouseButtonEvent &evt)
{
    // Widget::mouseReleaseEvent(e);
    return EventResult::UNHANDLED;
}



#if 0

InputField::InputField(Widget* parent, Vector tl, Vector br, std::string text)
    : TextField(parent, tl, br, text)
{
    focused = 0;
    this->text_valid = std::function([](std::string s){ return true; });
}

bool InputField::mousePressEvent(MouseEvent* e)
{
    bool in_abs_rect = inAbsRect({e->x, e->y});

    if (in_abs_rect && !focused)
    {
        state->focused = this;
        focused = true;

        SetFieldColor(gray_v);
        init_text = getText();
        return 0;
    }

    if (!in_abs_rect && focused)
        update_text();

    return 0;
}

bool InputField::keyboardEvent(KeyboardEvent* evt)
{
    if (evt->key == key_enter)
    {
        update_text();
        return 1;
    }

    std::string cur_text = getText();
    if (evt->key == SDLK_BACKSPACE)
        SetText(cur_text.substr(0, cur_text.size() - 1));
    else
        SetText(getText() + std::string(1, evt->key));

    return 1;
}

void InputField::update_text()
{
    SetFieldColor(blackV);
    focused = 0;
    if (state->focused == this) state->focused = nullptr;

    std::string new_text = getText();
    if (getText() != init_text)
    {
        if (text_valid(new_text))
        {
            action();
        }
        else
        {
            SetText(init_text);
        }

    }
}

void InputField::setValidator(std::function<bool(std::string)> text_valid)
{
    this->text_valid = text_valid;
}

#endif // 0

} // namespace hui