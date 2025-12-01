#include "mycanvas.h"
// #include "mywindow.h"

#include "hui/ui.hpp"

#include <cassert>

namespace pp {

ControlsTheme Canvas::GetControlsTheme() const { return ControlsTheme(); }


MyCanvas::MyCanvas(dr4::Window* window, dr4::Vec2f size)
    : size(size), window(window), texture(nullptr) {

    theme.shapeColor = dr4::Color(255, 0, 0);
}

ControlsTheme MyCanvas::GetControlsTheme() const { return theme; }

void MyCanvas::AddShape(Shape *shape) {
    shapes.push_back(shape);
}

void MyCanvas::DelShape(Shape *shape) {
    for (std::vector<Shape*>::iterator it = shapes.begin(); it != shapes.end(); ++it) {
        if (*it == shape) {
            shapes.erase(it);
            return;
        }
    }
}

void MyCanvas::setWidget(hui::Widget* w) {
    this->widget = w;
    this->texture = &w->GetFreshTexture();
}

void MyCanvas::ShapeChanged(Shape *shape) {
    widget->ForceRedraw();
}

dr4::Window *MyCanvas::GetWindow() { return window; }

void MyCanvas::DrawAllShapes() {
    for (Shape* sh: shapes) {
        texture->Draw(*sh);
    }
}

void MyCanvas::SetSelectedShape(Shape *shape) {}
Shape *MyCanvas::GetSelectedShape() const { return nullptr; }

} // namespace pp




CanvasWidget::CanvasWidget(hui::UI* ui, dr4::Vec2f pos, Widget* w)
    : MyContainer(ui, pos, w->GetSize())
{
    this->w = w;
    this->cvs = new pp::MyCanvas(GetUI()->GetWindow(), w->GetSize());
    cvs->setWidget(this);
    addChild(w);
}

void CanvasWidget::Redraw() const {
    w->DrawOn(GetTexture());
    cvs->DrawAllShapes();
}