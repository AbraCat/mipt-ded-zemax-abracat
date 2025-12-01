#include "button.h"
// #include "sdl-adapter.h"

#include "dr4/window.hpp"
#include "hui/ui.hpp"

#include <cassert>

using hui::EventResult;
using hui::MouseButtonEvent;
using hui::KeyEvent;

const double unpressColorCoeff = 0.7;
const int key_enter = 13;

extern dr4::Window* window;

// namespace hui {

TextField::TextField(hui::UI *state, dr4::Vec2f pos, dr4::Vec2f size)
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

void TextField::SetFieldColor(dr4::Color color) { this->color = color; ForceRedraw(); }//Redraw(); }
void TextField::SetText(std::string text) { this->text = text; ForceRedraw(); }//Redraw(); }
std::string TextField::getText() { return text; }




Button::Button(hui::UI *state, dr4::Vec2f pos, dr4::Vec2f size, dr4::Color color, std::string text)
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

void Button::imitatePress(bool down) {
    MouseButtonEvent evt;
    evt.pressed = down;
    evt.pos = GetPos() + GetSize() / 2;
    evt.button = dr4::MouseButtonType::LEFT;

    if (down) OnMouseDown(evt);
    else OnMouseUp(evt);
}

EventResult Button::OnMouseDown(MouseButtonEvent &evt)
{
    // Widget::mousePressEvent(e);

    if (!GetRect().Contains(evt.pos)) return EventResult::UNHANDLED;
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





ToggleButton::ToggleButton(hui::UI *state, dr4::Vec2f pos, dr4::Vec2f size, 
    dr4::Color color, std::string text)
    : Button(state, pos, size, color, text)
{
    //
}

EventResult ToggleButton::OnMouseDown(MouseButtonEvent &evt)
{
    // Widget::mousePressEvent(e);
    if (!GetRect().Contains(evt.pos)) return EventResult::UNHANDLED;

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



#if 1

static char KeycodeToChar(dr4::KeyCode code) {
    char chr = '\0';

    if (code >= dr4::KeyCode::KEYCODE_A && code <= dr4::KeyCode::KEYCODE_Z) {
        chr = 'a' + code - dr4::KeyCode::KEYCODE_A;
    }
    if (code >= dr4::KeyCode::KEYCODE_NUM0 && code <= dr4::KeyCode::KEYCODE_NUM9) {
        chr = '0' + code - dr4::KeyCode::KEYCODE_NUM0;
    }

    switch (code) {
        case dr4::KeyCode::KEYCODE_PERIOD: chr = '.'; break;
    }

    return chr;
}

InputField::InputField(hui::UI *state, dr4::Vec2f pos, dr4::Vec2f size, dr4::Color color, std::string text)
    : TextField(state, pos, size)
{
    focused = 0;
    this->text_valid = std::function([](std::string s){ return true; });
    SetFieldColor(color);
    SetText(text);
}

EventResult InputField::OnMouseDown(MouseButtonEvent &evt)
{
    bool in_abs_rect = GetRect().Contains(evt.pos);

    if (in_abs_rect && !focused)
    {
        // state->focused = this;
        GetUI()->ReportFocus(this);
        focused = true;

        SetFieldColor(dr4::Color(127, 127, 127)); // gray
        init_text = getText();
        return EventResult::UNHANDLED;
    }

    if (!in_abs_rect && focused)
        update_text();

    return EventResult::UNHANDLED;
}

EventResult InputField::OnKeyDown(KeyEvent &evt)
{
    if (evt.key == dr4::KeyCode::KEYCODE_ENTER)
    {
        update_text();
        return EventResult::HANDLED;
    }

    std::string cur_text = getText();
    if (evt.key == dr4::KeyCode::KEYCODE_BACKSPACE)
        SetText(cur_text.substr(0, cur_text.size() - 1));
    else {
        char chr = KeycodeToChar(evt.key);
        if (chr != '\0')
            SetText(getText() + std::string(1, KeycodeToChar(evt.key)));
    }

    return EventResult::HANDLED;
}

void InputField::update_text()
{
    SetFieldColor(dr4::Color(0, 0, 0)); // black
    focused = 0;
    // if (state->focused == this) state->focused = nullptr;
    if (GetUI()->GetFocused() == this) GetUI()->ReportFocus(nullptr);

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

// } // namespace hui