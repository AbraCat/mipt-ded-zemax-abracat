#include "button.h"
#include "colors.h"
#include "my-utils.h"

#include "dr4/window.hpp"
#include "hui/ui.hpp"

#include <cassert>

using hui::EventResult;
using hui::MouseButtonEvent;
using hui::KeyEvent;

const double unpressColorCoeff = 0.7;
const int letter_width = 17.3, text_h = 10;

extern dr4::Window* window;

TextField::TextField(hui::UI *state, dr4::Vec2f pos, dr4::Vec2f size)
    : Widget(state)
{
    SetPos(pos);
    SetSize(size);
    color = black_color;
    draw_border = true;
}

void TextField::Redraw() const
{
    dr4::Rectangle* rect = window->CreateRectangle();
    rect->SetSize(GetSize());
    rect->SetBorderColor(white_color);
    rect->SetFillColor(color);
    GetTexture().Draw(*rect);

    dr4::Text* text_drawable = window->CreateText();
    text_drawable->SetText(text);
    text_drawable->SetColor(white_color);
    text_drawable->SetPos(dr4::Vec2f(0, GetSize().y / 2 - text_h));
    GetTexture().Draw(*text_drawable);
}

void TextField::SetFieldColor(dr4::Color color) { this->color = color; ForceRedraw(); }
void TextField::SetText(std::string text) { this->text = text; ForceRedraw(); }
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
    if (!GetRect().Contains(evt.pos)) return EventResult::UNHANDLED;
    if (is_pressed) return EventResult::UNHANDLED;

    is_pressed = 1;
    SetFieldColor(press_color);
    ForceRedraw();
    action();
    return EventResult::UNHANDLED;
}

EventResult Button::OnMouseUp(MouseButtonEvent &evt)
{
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
    return EventResult::UNHANDLED;
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

    if (in_abs_rect)
    {
        GetUI()->ReportFocus(this);
        if (!focused) {
            focused = true;

            SetFieldColor(gray_color);
            init_text = getText();
            return EventResult::UNHANDLED;
        }
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
        char chr = KeycodeToChar(evt.key, evt.mods);
        std::string new_text = getText() + std::string(1, chr);
        float new_width = getTextBounds(GetUI()->GetWindow(), new_text).x;
        
        if (chr != '\0' && new_width < GetSize().x)
            SetText(new_text);
    }

    return EventResult::HANDLED;
}

void InputField::update_text()
{
    SetFieldColor(black_color);
    focused = 0;
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
    ForceRedraw();
}

void InputField::setValidator(std::function<bool(std::string)> text_valid)
{
    this->text_valid = text_valid;
}




ExclusiveButton::ExclusiveButton(hui::UI *state, dr4::Vec2f pos, dr4::Vec2f size, dr4::Color color, std::string text)
    : ToggleButton(state, pos, size, color, text) {
    //
}

void ExclusiveButton::action() {
    MyContainer* parent = dynamic_cast<MyContainer*>(GetParent());
    assert(parent != nullptr);

    for (hui::Widget* w: parent->children) {
        ToggleButton* button = dynamic_cast<ToggleButton*>(w);
        assert(button != nullptr);
        if (button != this && button->isPressed()) button->imitatePress(true);
    }
}
