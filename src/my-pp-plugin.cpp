#include "my-pp-plugin.h"
#include "pp/tool.hpp"

#include "dr4/window.hpp"

#include <cmath>
#include <cassert>

const dr4::Color shape_color(127, 0, 127), text_color(255, 0, 0), text_border_col(0, 0, 0);

extern "C" cum::Plugin *CreatePlugin() { return new cum::AbraCat_pp_plugin(); }


static void drawRectBorder(dr4::Rect2f rect, dr4::Texture& texture, dr4::Window* window, dr4::Color color) {
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


    top_line->SetColor(color);
    bottom_line->SetColor(color);
    left_line->SetColor(color);
    right_line->SetColor(color);
    texture.Draw(*top_line);
    texture.Draw(*bottom_line);
    texture.Draw(*left_line);
    texture.Draw(*right_line);
}

static char KeycodeToChar(dr4::KeyCode code, uint16_t mods) {
    char chr = '\0';
    bool shift = mods & dr4::KeyMode::KEYMOD_SHIFT, caps = mods & dr4::KeyMode::KEYMOD_CAPS;
    bool capital = shift && !caps || !shift && caps;

    #define RANGE(code_lft, code_rgt, chr_lft, chr_lft_capital)\
        if (code >= dr4::KeyCode::KEYCODE_ ## code_lft && code <= dr4::KeyCode::KEYCODE_ ## code_rgt) {\
            chr = (capital ? chr_lft_capital : chr_lft) + code - dr4::KeyCode::KEYCODE_ ## code_lft;\
        }
    #define CASE(dr4_code, character, char_capital)\
        case dr4::KeyCode::KEYCODE_ ## dr4_code: chr = (capital ? char_capital : character); break;

    RANGE(A, Z, 'a', 'A')

    switch (code) {
        CASE(NUM0, '0', ')')
        CASE(NUM1, '1', '!')
        CASE(NUM2, '2', '@')
        CASE(NUM3, '3', '#')
        CASE(NUM4, '4', '$')
        CASE(NUM5, '5', '%')
        CASE(NUM6, '6', '^')
        CASE(NUM7, '7', '&')
        CASE(NUM8, '8', '*')
        CASE(NUM9, '9', '(')

        CASE(SEMICOLON, ';', ':')
        CASE(COMMA, ',', '<')
        CASE(PERIOD, '.', '>')
        CASE(LBRACKET, '[', '{')
        CASE(RBRACKET, ']', '}')
        CASE(QUOTE, '\'', '"')
        CASE(SLASH, '/', '?')
        CASE(BACKSLASH, '\\', '|')
        CASE(TILDE, '`', '~')
        CASE(EQUAL, '=', '+')
        CASE(HYPHEN, '-', '_')
        CASE(SPACE, ' ', ' ')
    }

    #undef RANGE
    #undef CASE
    return chr;
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

bool TextShape::OnKeyDown(const dr4::Event::KeyEvent &evt) {
    if (evt.sym == dr4::KeyCode::KEYCODE_ENTER) {
        selected = false;
    }
    else if (evt.sym == dr4::KeyCode::KEYCODE_BACKSPACE) {
        if (text.size() > 0) text = text.substr(0, text.size() - 1);
    }
    else {
        char chr = KeycodeToChar(evt.sym, evt.mods);
        if (chr != '\0') text += chr;
    }

    canvas->ShapeChanged(this);
    return true;
}

void TextShape::DrawOn(dr4::Texture &texture) const {
    if (selected) {
        dr4::Rect2f rect(pos, size);
        drawRectBorder(rect, texture, window, text_border_col);
    }

    dr4::Text* text_drawable = window->CreateText();
    dr4::Vec2f text_pos(GetPos().x + std::min(0.0f, GetSize().x), GetPos().y + std::min(0.0f, GetSize().y));
    text_drawable->SetPos(text_pos);
    text_drawable->SetText(text);
    text_drawable->SetColor(text_color);
    texture.Draw(*text_drawable);
}






TextTool::TextTool() : MyTool() {
    //
}

std::string_view TextTool::Icon() const { return "T"; }
std::string_view TextTool::Name() const { return "text"; }

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
std::string_view RectTool::Name() const { return "rect"; }

MyShape* RectTool::createShape() { return new RectShape(canvas); }



CircleTool::CircleTool() : MyTool() {
    //
}

std::string_view CircleTool::Icon() const { return "C"; }
std::string_view CircleTool::Name() const { return "circle"; }

MyShape* CircleTool::createShape() { return new CircleShape(canvas); }



LineTool::LineTool() : MyTool() {
    //
}

std::string_view LineTool::Icon() const { return "L"; }
std::string_view LineTool::Name() const { return "line"; }

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
