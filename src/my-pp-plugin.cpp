#include "my-pp-plugin.h"
#include "pp/tool.hpp"

#include "dr4/window.hpp"

#include <cmath>
#include <cassert>

const dr4::Color shape_color(255, 0, 0);

extern "C" cum::Plugin *CreatePlugin() { return new cum::AbraCat_pp_plugin(); }

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
    
    tex.Draw(*line);
    delete line;
}





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

    is_drawing = true;
    return true;
}

bool MyTool::OnMouseUp(const dr4::Event::MouseButton &evt) {
    if (!is_selected || !is_drawing) return false;

    cur_shape->SetSize(evt.pos - cur_shape->GetPos());
    canvas->ShapeChanged(cur_shape);

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
