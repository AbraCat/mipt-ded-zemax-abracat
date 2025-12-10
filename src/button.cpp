#include "button.h"
#include "colors.h"
#include "my-utils.h"

#include "dr4/window.hpp"
#include "hui/ui.hpp"

#include <cassert>
#include <cmath>

using hui::EventResult;
using hui::MouseButtonEvent;
using hui::KeyEvent;

const double unpressColorCoeff = 0.7, cursor_time = 0.5;
const int letter_width = 17.3, text_h = 10, cursor_start_pad = 3, cursor_end_pad = 3;
const dr4::Color cursor_col(255, 255, 255);

extern dr4::Window* window;

TextField::TextField(hui::UI *state, dr4::Vec2f pos, dr4::Vec2f size)
    : Widget(state)
{
    SetPos(pos);
    SetSize(size);

    draw_rounded_angles = true;
    top_left_pad = bottom_right_pad = dr4::Vec2f();
    angle_radius = 10;
    color = dr4::Color(61, 61, 61);
    border_col = dr4::Color(127, 127, 127);
    draw_border = true;
}

void TextField::Redraw() const
{
    GetTexture().Clear(black_color);
    if (draw_rounded_angles) drawRoundedRect(dr4::Rect2f(top_left_pad,
        GetSize() - top_left_pad - bottom_right_pad), GetTexture(), GetUI()->GetWindow(),
        color, true, border_col, angle_radius);
    else {
        dr4::Rectangle* rect = GetUI()->GetWindow()->CreateRectangle();
        rect->SetPos(top_left_pad);
        rect->SetSize(GetSize() - top_left_pad - bottom_right_pad);
        rect->SetFillColor(color);
        rect->SetBorderColor(border_col);
        GetTexture().Draw(*rect);
        delete rect;
    }

    dr4::Text* text_drawable = window->CreateText();
    text_drawable->SetText(text);
    text_drawable->SetColor(white_color);
    text_drawable->SetPos(dr4::Vec2f(0, GetSize().y / 2 - text_h));
    text_drawable->SetFont(window->GetDefaultFont());
    GetTexture().Draw(*text_drawable);
}

void TextField::SetFieldColor(dr4::Color color) { this->color = color; ForceRedraw(); }
void TextField::SetText(std::string text) { this->text = text; ForceRedraw(); }
std::string TextField::getText() const { return text; }




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
    printf("handled\n");
    return EventResult::HANDLED;
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
    max_text_len = size.x;
    this->text_valid = std::function([](std::string s){ return true; });

    cursor_pos = 0;
    cursor_visible = true;

    SetFieldColor(dark_gray_color);
    SetText(text);
}

void InputField::Redraw() const {
    TextField::Redraw();
    if (focused && cursor_visible) drawCursor(GetTexture());
}

EventResult InputField::OnMouseDown(MouseButtonEvent &evt)
{
    bool in_rect = GetRect().Contains(evt.pos);

    if (in_rect)
    {
        GetUI()->ReportFocus(this);
        if (!focused) {
            focused = true;

            SetFieldColor(gray_color);
            init_text = getText();
            return EventResult::UNHANDLED;
        }
    }

    if (!in_rect && focused)
        update_text();

    return EventResult::UNHANDLED;
}

EventResult InputField::OnKeyDown(KeyEvent &evt)
{
    // if (evt.key == dr4::KeyCode::KEYCODE_ENTER)
    // {
    //     update_text();
    //     return EventResult::HANDLED;
    // }

    // std::string cur_text = getText();
    // if (evt.key == dr4::KeyCode::KEYCODE_BACKSPACE)
    //     SetText(cur_text.substr(0, cur_text.size() - 1));
    // else {
    //     char chr = KeycodeToChar(evt.key, evt.mods);
    //     std::string new_text = getText() + std::string(1, chr);
    //     float new_width = getTextBounds(GetUI()->GetWindow(), new_text).x;
        
    //     if (chr != '\0' && new_width < max_text_len)
    //         SetText(new_text);
    // }

    // return EventResult::HANDLED;

    switch (evt.key) {
        case dr4::KeyCode::KEYCODE_ENTER:
            update_text();
            return EventResult::HANDLED;
        case dr4::KeyCode::KEYCODE_LEFT:
            if (cursor_pos < getText().size()) ++cursor_pos;
            break;
        case dr4::KeyCode::KEYCODE_RIGHT:
            if (cursor_pos > 0) --cursor_pos;
            break;
        case dr4::KeyCode::KEYCODE_BACKSPACE: {
            std::string cur_text = getText();
            if (cur_text.size() <= cursor_pos) break;
            SetText(cur_text.substr(0, cur_text.size() - cursor_pos - 1) +
                cur_text.substr(cur_text.size() - cursor_pos, cursor_pos));
            break;
        }
        default:
            char chr = KeycodeToChar(evt.key, evt.mods);
            std::string cur_text = getText();
            std::string new_text = cur_text.substr(0, cur_text.size() - cursor_pos) + chr +
                cur_text.substr(cur_text.size() - cursor_pos, cursor_pos);

            if (inBounds(new_text) && chr != '\0') SetText(new_text);
    }   

    ForceRedraw();
    return hui::EventResult::HANDLED;
}

hui::EventResult InputField::OnIdle(hui::IdleEvent &evt) {
    if (!focused || evt.absTime - prev_cursor_time < cursor_time) return hui::EventResult::UNHANDLED;
    
    cursor_visible = !cursor_visible;
    prev_cursor_time = evt.absTime;
    ForceRedraw();
    return hui::EventResult::UNHANDLED;
}

void InputField::update_text()
{
    SetFieldColor(dark_gray_color);
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

void InputField::drawCursor(dr4::Texture& texture) const {
    std::string cur_text = getText();
    std::string text_before_cursor = cur_text.substr(0, cur_text.size() - cursor_pos);
    dr4::Vec2f text_bounds = getTextBounds(window, text_before_cursor);

    dr4::Line* cursor_line = window->CreateLine();
    cursor_line->SetStart(dr4::Vec2f(text_bounds.x, cursor_start_pad + top_left_pad.y));
    cursor_line->SetEnd(dr4::Vec2f(text_bounds.x, GetSize().y - cursor_end_pad - bottom_right_pad.y));
    cursor_line->SetColor(cursor_col);
    cursor_line->SetThickness(3);

    texture.Draw(*cursor_line);
    delete cursor_line;
}

bool InputField::inBounds(std::string new_text) {
    dr4::Vec2f bounds = getTextBounds(window, new_text);
    return bounds.x < std::fabs(GetSize().x);
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
