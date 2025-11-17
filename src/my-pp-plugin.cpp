#include "my-pp-plugin.h"
#include "pp/tool.hpp"

#include "dr4/window.hpp"

extern dr4::Window* window = nullptr;

namespace pp {

bool Shape::OnMouseDown(const dr4::Event::MouseButton &evt) { return false; }
bool Shape::OnMouseUp(const dr4::Event::MouseButton &evt) { return false; }
bool Shape::OnMouseMove(const dr4::Event::MouseMove &evt) { return false; }

void Shape::OnSelect() {}
void Shape::OnDeselect() {}

void Shape::DrawOn(dr4::Texture &tex) {}



RectShape::RectShape() {
    selected = false;

    pos = size = dr4::Vec2f();
}

void RectShape::SetPos(dr4::Vec2f pos) { this->pos = pos; }
dr4::Vec2f RectShape::GetPos() const { return pos; }
void RectShape::SetSize(dr4::Vec2f size) { this->size = size; }
dr4::Vec2f RectShape::GetSize() const { return size; }

bool RectShape::OnMouseDown(const dr4::Event::MouseButton &evt) {
    return false;

    // if (!selected) return false;

    // pos = evt.pos;
    // return true;
}

bool RectShape::OnMouseUp(const dr4::Event::MouseButton &evt) {
    return false;

    // if (!selected) return false;
    // size = evt.pos - pos;
}

bool RectShape::OnMouseMove(const dr4::Event::MouseMove &evt) {
    return false;

    // if (!selected) return false;
    // size = evt.pos - pos;
}

void RectShape::OnSelect() { selected = true; }
void RectShape::OnDeselect() { selected = false; }

void RectShape::DrawOn(dr4::Texture &tex) const {
    dr4::Rectangle* rect = window->CreateRectangle();
    rect->SetPos(pos);
    rect->SetSize(size);
    rect->SetFillColor(dr4::Color(255, 0, 0));
    tex.Draw(*rect);
}




std::string_view Tool::Icon() const { return ""; };
std::string_view Tool::Name() const { return ""; }
bool Tool::IsCurrentlyDrawing() const { return false; }

void Tool::OnStart() {}
void Tool::OnBreak() {}
void Tool::OnEnd() {}

bool Tool::OnMouseDown(const dr4::Event::MouseButton &evt) { return false; }
bool Tool::OnMouseUp(const dr4::Event::MouseButton &evt) { return false; }
bool Tool::OnMouseMove(const dr4::Event::MouseMove &evt) { return false; }





RectTool::RectTool() {
    is_drawing = is_selected = false;
    cur_shape = nullptr;
    canvas = nullptr;
}

void RectTool::SetCanvas(Canvas* canvas) { this->canvas = canvas; }

std::string_view RectTool::Icon() const { return "R"; };
std::string_view RectTool::Name() const { return "Rect"; }
bool RectTool::IsCurrentlyDrawing() const { return is_drawing; }

void RectTool::OnStart() { is_selected = true; }

void RectTool::OnBreak() {
    delete cur_shape;
    is_drawing = false;
    // redraw ?
}

void RectTool::OnEnd() { is_selected = false; }

bool RectTool::OnMouseDown(const dr4::Event::MouseButton &evt) { 
    if (!is_selected) return false;

    cur_shape = new RectShape();
    cur_shape->SetPos(evt.pos);
    canvas->AddShape(cur_shape);

    is_drawing = true;
    return true;
}

bool RectTool::OnMouseUp(const dr4::Event::MouseButton &evt) {
    if (!is_selected || !is_drawing) return false;

    cur_shape->SetSize(evt.pos - cur_shape->GetPos());
    canvas->ShapeChanged(cur_shape);

    cur_shape = nullptr;
    is_drawing = false;
    return true;
}

bool RectTool::OnMouseMove(const dr4::Event::MouseMove &evt) {
    if (!is_selected || !is_drawing) return false;

    cur_shape->SetSize(evt.pos - cur_shape->GetPos());
    canvas->ShapeChanged(cur_shape);
    return true;
}





std::vector<pp::Tool*> PPToolPlugin::CreateTools(Canvas *cvs) { return std::vector<pp::Tool*>(); }



AbraCat_pp_plugin::AbraCat_pp_plugin() {
    name = description = "AbraCat pp plugin";
}

std::vector<pp::Tool*> AbraCat_pp_plugin::CreateTools(Canvas *cvs) {
    RectTool* rect_tool = new RectTool();
    rect_tool->SetCanvas(cvs);

    return std::vector<pp::Tool*>(1, rect_tool);
}

std::string_view AbraCat_pp_plugin::GetName() const { return name; }
std::string_view AbraCat_pp_plugin::GetIdentifier() const { return name; }
std::string_view &AbraCat_pp_plugin::GetDescription() const {std::string_view str(description); return str; }
std::vector<std::string_view> AbraCat_pp_plugin::GetDependencies() const { return std::vector<std::string_view>(); }
std::vector<std::string_view> AbraCat_pp_plugin::GetConflicts() const { return std::vector<std::string_view>(); }
void AbraCat_pp_plugin::AfterLoad() {}



extern "C" AbraCat_pp_plugin* Create_PP_Plugin(void) { return new AbraCat_pp_plugin(); }

} // namespace pp
