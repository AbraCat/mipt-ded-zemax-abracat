#ifndef MY_BUTTON_H
#define MY_BUTTON_H

#include "widget.h"
#include "reactor.h"

#include "hui/widget.hpp"

#include <cstdio>
#include <string>

namespace hui {

class TextField : public hui::Widget
{
public:
    TextField(hui::State *state, dr4::Vec2f pos, dr4::Vec2f size);
    // virtual void updateTexture() override;
    virtual void Redraw() const override;

    void SetFieldColor(dr4::Color color);
    void SetText(std::string text);
    std::string getText();

private:
    bool draw_border;
    dr4::Color color;
    std::string text;
};

// class InputField : public TextField
// {
// public:
//     InputField(Widget* parent, Vector tl, Vector br, std::string text);

//     virtual bool mousePressEvent(MouseEvent* e) override;
//     virtual bool keyboardEvent(KeyboardEvent* evt) override;

//     void update_text();
//     virtual void action() = 0;

//     void setValidator(std::function<bool(std::string)> text_valid);

//     bool focused;
//     std::string init_text;
//     std::function<bool(std::string)> text_valid;
// };

class Button : public TextField
{
public:
    Button(hui::State *state, dr4::Vec2f pos, dr4::Vec2f size, dr4::Color color, std::string text);

    virtual EventResult OnMouseDown(MouseButtonEvent &evt) override;
    virtual EventResult OnMouseUp(MouseButtonEvent &evt) override;

    virtual void action() = 0;
    virtual void unpress();

    bool is_pressed;
    dr4::Color press_color, unpress_color;
    std::string text;
};

class ToggleButton : public Button
{
public:
    ToggleButton(hui::State *state, dr4::Vec2f pos, dr4::Vec2f size, dr4::Color color, std::string text);

    virtual EventResult OnMouseDown(MouseButtonEvent &evt) override;
    virtual EventResult OnMouseUp(MouseButtonEvent &evt) override;

    virtual void deactivate() = 0;
};

} // namespace hui

#endif // MY_BUTTON_H