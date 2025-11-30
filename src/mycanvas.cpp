#include "mycanvas.h"
// #include "mywindow.h"

#include <cassert>

namespace pp {

ControlsTheme Canvas::GetControlsTheme() const { return ControlsTheme(); }


MyCanvas::MyCanvas(dr4::Window* window, dr4::Vec2f size)
    : size(size), window(window), texture(nullptr) {
    // texture = window->CreateTexture();
    // texture->SetSize(size);
    // texture->SetPos(dr4::Vec2f());

    theme.shapeColor = dr4::Color(255, 0, 0);
}

ControlsTheme MyCanvas::GetControlsTheme() const { return theme; }

void MyCanvas::AddShape(Shape *shape) {
    shapes.push_back(shape);
    // draw
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

    // window->Draw(*texture);
}

void MyCanvas::SetSelectedShape(Shape *shape) {} // TODO
Shape *MyCanvas::GetSelectedShape() const { return nullptr; }

} // namespace pp