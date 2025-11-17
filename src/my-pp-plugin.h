#ifndef ABRACAT_PP_PLUGIN_H
#define ABRACAT_PP_PLUGIN_H

#include "pp/shape.hpp"
#include "pp/canvas.hpp"
#include "cum/ifc/pp.hpp"

#include <vector>

namespace pp {

class RectShape : public Shape {
public:
    RectShape();

    void SetPos(dr4::Vec2f pos) override;
    dr4::Vec2f GetPos() const override;
    void SetSize(dr4::Vec2f size);
    dr4::Vec2f GetSize() const;

    virtual bool OnMouseDown(const dr4::Event::MouseButton &evt) override;
    virtual bool OnMouseUp(const dr4::Event::MouseButton &evt) override;
    virtual bool OnMouseMove(const dr4::Event::MouseMove &evt) override;

    virtual void OnSelect() override;
    virtual void OnDeselect() override;

    virtual void DrawOn(dr4::Texture &tex) const override;

private:
    bool selected;
    dr4::Vec2f pos, size;
};

class RectTool : public Tool {
public:
    RectTool();
    void SetCanvas(Canvas* canvas);

    virtual std::string_view Icon() const override;
    virtual std::string_view Name() const override;
    virtual bool IsCurrentlyDrawing() const override;

    virtual void OnStart() override;
    virtual void OnBreak() override;
    virtual void OnEnd() override;
    virtual bool OnMouseDown(const dr4::Event::MouseButton &evt) override;
    virtual bool OnMouseUp(const dr4::Event::MouseButton &evt) override;
    virtual bool OnMouseMove(const dr4::Event::MouseMove &evt) override;

private:
    bool is_drawing, is_selected;
    RectShape* cur_shape;
    Canvas* canvas;
};

class MyCanvas : public Canvas {
public:
    MyCanvas(dr4::Window* window);

    virtual ControlsTheme GetControlsTheme() const;
    virtual void AddShape(Shape *shape) override;
    virtual void DelShape(Shape *shape) override;
    virtual void ShapeChanged(Shape *shape) override;
    virtual dr4::Window *GetWindow() override;

private:
    dr4::Window* window;
    std::vector<Shape*> shapes;
};


class AbraCat_pp_plugin : public PPToolPlugin
{
public:
    AbraCat_pp_plugin();
    virtual std::vector<pp::Tool*> CreateTools(Canvas *cvs) override;

    virtual std::string_view GetIdentifier() const override;
    virtual std::string_view GetName() const override;
    virtual std::string_view &GetDescription() const override;
    virtual std::vector<std::string_view> GetDependencies() const override;
    virtual std::vector<std::string_view> GetConflicts() const override;
    virtual void AfterLoad() override;

private:
    std::string name, description;
    // std::string_view descr_view;
};

extern "C" AbraCat_pp_plugin* Create_PP_Plugin(void);


} // namespace pp


#endif // ABRACAT_PP_PLUGIN_H