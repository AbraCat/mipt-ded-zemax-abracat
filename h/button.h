#ifndef MY_BUTTON_H
#define MY_BUTTON_H

#include "wcontainer.h"

#include "hui/widget.hpp"

#include <cstdio>
#include <string>

class TextField : public hui::Widget
{
public:
    TextField(hui::UI *state, dr4::Vec2f pos, dr4::Vec2f size);
    virtual ~TextField() noexcept override = default;
    virtual void Redraw() const override;

    void setRectPadding(double padding) { this->rect_padding = padding; }
    void SetFieldColor(dr4::Color color);
    void SetText(std::string text);
    std::string getText() const;

private:
    bool draw_border;
    double rect_padding;
    dr4::Color color;
    std::string text;
};

class InputField : public TextField
{
public:
    InputField(hui::UI *state, dr4::Vec2f pos, dr4::Vec2f size, dr4::Color color, std::string text);
    void Redraw() const override;    

    virtual hui::EventResult OnMouseDown(hui::MouseButtonEvent &evt) override;
    virtual hui::EventResult OnKeyDown(hui::KeyEvent &evt) override;
    virtual hui::EventResult OnIdle(hui::IdleEvent &evt) override;

    void update_text();
    virtual void action() = 0;
    bool inBounds(std::string new_text);
    void drawCursor(dr4::Texture& texture) const;

    void setMaxTextLen(int len) { max_text_len = len; }
    void setValidator(std::function<bool(std::string)> text_valid);

protected:
    bool focused, cursor_visible;
    int max_text_len, cursor_pos;
    double prev_cursor_time;
    std::string init_text;
    std::function<bool(std::string)> text_valid;
};

class Button : public TextField
{
public:
    Button(hui::UI *state, dr4::Vec2f pos, dr4::Vec2f size, dr4::Color color, std::string text);
    virtual ~Button() noexcept override = default;

    void imitatePress(bool down);
    virtual hui::EventResult OnMouseDown(hui::MouseButtonEvent &evt) override;
    virtual hui::EventResult OnMouseUp(hui::MouseButtonEvent &evt) override;

    bool isPressed() const { return is_pressed; }
    virtual void action() = 0;
    virtual void unpress();

    bool is_pressed;
    dr4::Color press_color, unpress_color;
    std::string text;
};

class ToggleButton : public Button
{
public:
    ToggleButton(hui::UI *state, dr4::Vec2f pos, dr4::Vec2f size, dr4::Color color, std::string text);
    virtual ~ToggleButton() noexcept override = default;

    virtual hui::EventResult OnMouseDown(hui::MouseButtonEvent &evt) override;
    virtual hui::EventResult OnMouseUp(hui::MouseButtonEvent &evt) override;

    virtual void deactivate() = 0;
};



class ExclusiveButton : public ToggleButton {
public:
    ExclusiveButton(hui::UI *state, dr4::Vec2f pos, dr4::Vec2f size, dr4::Color color, std::string text);
    virtual void action() override;
};

#endif // MY_BUTTON_H