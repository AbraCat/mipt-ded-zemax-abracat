#include "mycanvas.h"
#include "mywindow.h"

#include <cassert>

namespace pp {

ControlsTheme Canvas::GetControlsTheme() const { return ControlsTheme(); }


MyCanvas::MyCanvas(dr4::Window* window, dr4::Vec2f size, dr4::Texture* texture)
    : size(size), window(window), texture(texture) {
    // texture = window->CreateTexture();
    // texture->SetSize(size);
    // texture->SetPos(dr4::Vec2f());
}

ControlsTheme MyCanvas::GetControlsTheme() const { return ControlsTheme(); }

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

void MyCanvas::ShapeChanged(Shape *shape) {
    // redraw
}

dr4::Window *MyCanvas::GetWindow() { return window; }

void MyCanvas::DrawAllShapes() {
    for (Shape* sh: shapes) {
        texture->Draw(*sh);
    }

    // window->Draw(*texture);
}

} // namespace pp