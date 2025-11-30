#include "my-pp-plugin.h"
#include "pp/tool.hpp"

#include "dr4/window.hpp"

#include <cmath>
#include <cassert>

const dr4::Color shape_color(255, 0, 0);

extern "C" cum::Plugin *CreatePlugin() { return new cum::AbraCat_pp_plugin(); }


static void drawRectBorder(dr4::Rect2f rect, dr4::Texture& texture, dr4::Window* window) {
    dr4::Line *top_line = window->CreateLine(), *bottom_line = window->CreateLine(),
        *left_line = window->CreateLine(), *right_line = window->CreateLine();
    dr4::Vec2f p1 = rect.pos, p2 = rect.pos + rect.size;

    top_line->SetStart(dr4::Vec2f(p1.x, p1.y));
    top_line->SetEnd(dr4::Vec2f(p2.x, p1.y));
    bottom_line->SetStart(dr4::Vec2f(p1.x, p2.y));
    bottom_line->SetEnd(dr4::Vec2f(p2.x, p2.y));
    left_line->SetStart(dr4::Vec2f(p1.x, p1.y));
    left_line->SetEnd(dr4::Vec2f(p1.x, p2.y));
    right_line->SetStart(dr4::Vec2f(p2.x, p1.y));
    right_line->SetEnd(dr4::Vec2f(p2.x, p2.y));


    top_line->SetColor(dr4::Color(255, 0, 0));
    bottom_line->SetColor(dr4::Color(255, 0, 0));
    left_line->SetColor(dr4::Color(255, 0, 0));
    right_line->SetColor(dr4::Color(255, 0, 0));
    texture.Draw(*top_line);
    texture.Draw(*bottom_line);
    texture.Draw(*left_line);
    texture.Draw(*right_line);
}

namespace pp {

MyShape::MyShape(Canvas* canvas) : canvas(canvas), window(canvas->GetWindow()) {
    selected = false;
    pos = size = dr4::Vec2f();
}

void MyShape::SetPos(dr4::Vec2f pos) { this->pos = pos; }
dr4::Vec2f MyShape::GetPos() const { return pos; }
void MyShape::SetSize(dr4::Vec2f size) { this->size = size; }
dr4::Vec2f MyShape::GetSize() const { return size; }

void MyShape::OnSelect() { selected = true; }
void MyShape::OnDeselect() { selected = false; }




RectShape::RectShape(Canvas* canvas) : MyShape(canvas) {
    //
}

void RectShape::DrawOn(dr4::Texture &tex) const {
    dr4::Rectangle* rect = window->CreateRectangle();
    rect->SetPos(pos);
    rect->SetSize(size);
    rect->SetFillColor(dr4::Color(255, 0, 0));
    tex.Draw(*rect);
}

CircleShape::CircleShape(Canvas* canvas) : MyShape(canvas) {
    //
}

void CircleShape::DrawOn(dr4::Texture &tex) const {
    dr4::Circle* circle = window->CreateCircle();

    circle->SetCenter(pos + size / 2);
    circle->SetRadius(std::sqrt(size.x * size.x + size.y * size.y) / 2);
    circle->SetFillColor(shape_color);

    tex.Draw(*circle);
    delete circle;
}

LineShape::LineShape(Canvas* canvas) : MyShape(canvas) {
    //
}

void LineShape::DrawOn(dr4::Texture &tex) const {
    dr4::Line* line = window->CreateLine();

    line->SetStart(pos);
    line->SetEnd(pos + size);
    line->SetColor(shape_color);
    line->SetThickness(3);
    
    tex.Draw(*line);
    delete line;
}

TextShape::TextShape(Canvas* cvs) : MyShape(cvs) {
    text = "text";
}

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

bool TextShape::OnKeyDown(const dr4::Event::KeyEvent &evt) {
    if (evt.sym == dr4::KeyCode::KEYCODE_ENTER) {
        selected = false;
    }
    else if (evt.sym == dr4::KeyCode::KEYCODE_BACKSPACE) {
        if (text.size() > 0) text = text.substr(0, text.size() - 1);
    }
    else {
        text += KeycodeToChar(evt.sym);
    }

    canvas->ShapeChanged(this);
    return true;
}

void TextShape::DrawOn(dr4::Texture &texture) const {
    if (selected) {
        dr4::Rect2f rect(pos, size);
        drawRectBorder(rect, texture, window);
    }

    dr4::Text* text_drawable = window->CreateText();
    dr4::Vec2f text_pos(GetPos().x + std::min(0.0f, GetSize().x), GetPos().y + std::min(0.0f, GetSize().y));
    text_drawable->SetPos(text_pos);
    text_drawable->SetText(text);
    texture.Draw(*text_drawable);
}






TextTool::TextTool() : MyTool() {
    //
}

std::string_view TextTool::Icon() const { return "T"; }
std::string_view TextTool::Name() const { return "Text"; }

bool TextTool::OnMouseDown(const dr4::Event::MouseButton &evt) {
    if (cur_shape != nullptr) {
        dr4::Event::KeyEvent key_evt;
        key_evt.mods = 0;
        key_evt.sym = dr4::KeyCode::KEYCODE_ENTER;
        this->OnKeyDown(key_evt);
    }

    return MyTool::OnMouseDown(evt);
}

bool TextTool::OnMouseUp(const dr4::Event::MouseButton &evt) {
    if (!is_selected || !is_drawing) return false;

    cur_shape->SetSize(evt.pos - cur_shape->GetPos());
    canvas->ShapeChanged(cur_shape);

    is_drawing = false;
    return true;
}

bool TextTool::OnKeyDown(const dr4::Event::KeyEvent &evt) {
    if (cur_shape != nullptr) {
        if (evt.sym == dr4::KeyCode::KEYCODE_ENTER) {
            bool result = cur_shape->OnKeyDown(evt);
            cur_shape->OnDeselect();
            canvas->ShapeChanged(cur_shape);

            cur_shape = nullptr;
            return result;
        }

        return cur_shape->OnKeyDown(evt);
    }

    return false;
}

MyShape* TextTool::createShape() { return new TextShape(canvas); }




MyTool::MyTool() {
    is_drawing = is_selected = false;
    canvas = nullptr;
    cur_shape = nullptr;
}

void MyTool::SetCanvas(Canvas* canvas) { this->canvas = canvas; }
bool MyTool::IsCurrentlyDrawing() const { return is_drawing; }

void MyTool::OnStart() { is_selected = true; }
void MyTool::OnEnd() { is_selected = false; }

void MyTool::OnBreak() {
    delete cur_shape;
    is_drawing = false;
    // redraw ?
}

bool MyTool::OnMouseDown(const dr4::Event::MouseButton &evt) { 
    if (!is_selected) return false;

    cur_shape = createShape();
    cur_shape->SetPos(evt.pos);
    canvas->AddShape(cur_shape);
    cur_shape->OnSelect();

    is_drawing = true;
    return true;
}

bool MyTool::OnMouseUp(const dr4::Event::MouseButton &evt) {
    if (!is_selected || !is_drawing) return false;

    cur_shape->SetSize(evt.pos - cur_shape->GetPos());
    canvas->ShapeChanged(cur_shape);
    cur_shape->OnDeselect();

    cur_shape = nullptr;
    is_drawing = false;
    return true;
}

bool MyTool::OnMouseMove(const dr4::Event::MouseMove &evt) {
    if (!is_selected || !is_drawing) return false;

    cur_shape->SetSize(evt.pos - cur_shape->GetPos());
    canvas->ShapeChanged(cur_shape);
    return true;
}




RectTool::RectTool() : MyTool() {
    //
}

std::string_view RectTool::Icon() const { return "R"; }
std::string_view RectTool::Name() const { return "Rect"; }

MyShape* RectTool::createShape() { return new RectShape(canvas); }



CircleTool::CircleTool() : MyTool() {
    //
}

std::string_view CircleTool::Icon() const { return "C"; }
std::string_view CircleTool::Name() const { return "Circle"; }

MyShape* CircleTool::createShape() { return new CircleShape(canvas); }



LineTool::LineTool() : MyTool() {
    //
}

std::string_view LineTool::Icon() const { return "L"; }
std::string_view LineTool::Name() const { return "Line"; }

MyShape* LineTool::createShape() { return new LineShape(canvas); }


} // namespace pp

namespace cum {

cum::AbraCat_pp_plugin::AbraCat_pp_plugin() {
    name = description = "AbraCat pp plugin";
}

std::vector<std::unique_ptr<pp::Tool>> cum::AbraCat_pp_plugin::CreateTools(pp::Canvas *cvs) {
    std::vector<std::unique_ptr<pp::Tool>> tools;
    tools.push_back(std::unique_ptr<pp::Tool>(new pp::RectTool()));
    tools.push_back(std::unique_ptr<pp::Tool>(new pp::CircleTool())); 
    tools.push_back(std::unique_ptr<pp::Tool>(new pp::LineTool()));
    tools.push_back(std::unique_ptr<pp::Tool>(new pp::TextTool()));

    for (std::unique_ptr<pp::Tool>& tl: tools) {
        pp::MyTool* my_tool = dynamic_cast<pp::MyTool*>(&*tl);
        assert(my_tool != nullptr);
        my_tool->SetCanvas(cvs);
    }

    return tools;
}

std::string_view cum::AbraCat_pp_plugin::GetName() const { return name; }
std::string_view cum::AbraCat_pp_plugin::GetIdentifier() const { return name; }
std::string_view cum::AbraCat_pp_plugin::GetDescription() const {std::string_view str(description); return str; }
std::vector<std::string_view> cum::AbraCat_pp_plugin::GetDependencies() const { return std::vector<std::string_view>(); }
std::vector<std::string_view> cum::AbraCat_pp_plugin::GetConflicts() const { return std::vector<std::string_view>(); }
void cum::AbraCat_pp_plugin::AfterLoad() {}

} // namespace cum
