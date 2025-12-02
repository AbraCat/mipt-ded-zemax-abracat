#ifndef MY_CANVAS_H
#define MY_CANVAS_H

#include "pp/shape.hpp"
#include "pp/canvas.hpp"
#include "hui/widget.hpp"

#include "wcontainer.h"

#include <vector>

namespace pp {

class MyCanvas : public Canvas {
public:
    MyCanvas(dr4::Window* window, dr4::Vec2f size);

    virtual ControlsTheme GetControlsTheme() const;
    virtual void AddShape(Shape *shape) override;
    virtual void DelShape(Shape *shape) override;
    virtual void ShapeChanged(Shape *shape) override;
    virtual dr4::Window *GetWindow() override;

    void setWidget(hui::Widget* w);
    hui::Widget* getWidget() const { return widget; }
    // void setTexture(dr4::Texture* texture) { this->texture = texture; }
    // dr4::Texture* getTexture() const { return texture; }
    virtual void SetSelectedShape(Shape *shape) override;
    virtual Shape *GetSelectedShape() const override;

    void DrawAllShapes();

private:
    dr4::Vec2f size;
    dr4::Window* window;
    dr4::Texture* texture;
    hui::Widget* widget;
    std::vector<Shape*> shapes;
    ControlsTheme theme;
};

} // namespace pp

class CanvasWidget : public MyContainer {
public:
    CanvasWidget(hui::UI* ui, dr4::Vec2f pos, Widget* w);
    virtual void Redraw() const override;
    pp::MyCanvas* getCanvas() const { return cvs; }

protected:
    Widget* w;
    pp::MyCanvas* cvs;
};


#endif // MY_CANVAS_H