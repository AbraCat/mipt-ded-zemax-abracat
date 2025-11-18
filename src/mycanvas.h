#ifndef MY_CANVAS_H
#define MY_CANVAS_H

#include "pp/shape.hpp"
#include "pp/canvas.hpp"

#include <vector>

namespace pp {

class MyCanvas : public Canvas {
public:
    MyCanvas(dr4::Window* window, dr4::Vec2f size, dr4::Texture* texture);

    virtual ControlsTheme GetControlsTheme() const;
    virtual void AddShape(Shape *shape) override;
    virtual void DelShape(Shape *shape) override;
    virtual void ShapeChanged(Shape *shape) override;
    virtual dr4::Window *GetWindow() override;

    void DrawAllShapes();

private:
    dr4::Vec2f size;
    dr4::Window* window;
    std::vector<Shape*> shapes;
    dr4::Texture* texture;
    ControlsTheme theme;
};

} // namespace pp


#endif // MY_CANVAS_H