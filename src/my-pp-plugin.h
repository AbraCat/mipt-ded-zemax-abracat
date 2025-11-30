#ifndef ABRACAT_PP_PLUGIN_H
#define ABRACAT_PP_PLUGIN_H

#include "pp/shape.hpp"
#include "pp/canvas.hpp"
#include "cum/ifc/pp.hpp"

#include "dr4/window.hpp"

#include <vector>

namespace pp {

class MyShape : public Shape {
public:
    MyShape(Canvas* canvas);

    void SetPos(dr4::Vec2f pos) override;
    dr4::Vec2f GetPos() const override;
    void SetSize(dr4::Vec2f size);
    dr4::Vec2f GetSize() const;

    virtual void OnSelect() override;
    virtual void OnDeselect() override;

protected:
    bool selected;
    dr4::Vec2f pos, size;
    dr4::Window* window;
    Canvas* canvas;
};

class RectShape : public MyShape {
public:
    RectShape(Canvas* canvas);
    virtual void DrawOn(dr4::Texture &tex) const override;
};

class CircleShape : public MyShape {
public:
    CircleShape(Canvas* canvas);
    virtual void DrawOn(dr4::Texture &tex) const override;
};

class LineShape : public MyShape {
public:
    LineShape(Canvas* canvas);
    virtual void DrawOn(dr4::Texture &tex) const override;
};

class TextShape : public MyShape {
public:
    TextShape(Canvas* cvs);

    virtual bool OnKeyDown(const dr4::Event::KeyEvent &evt) override;
    virtual void DrawOn(dr4::Texture &) const override;
private:
    std::string text;
};



class MyTool : public Tool {
public:
    MyTool();
    virtual MyShape* createShape() = 0;

    void SetCanvas(Canvas* canvas);
    virtual bool IsCurrentlyDrawing() const override;

    virtual void OnStart() override;
    virtual void OnEnd() override;
    virtual void OnBreak() override;

    virtual bool OnMouseDown(const dr4::Event::MouseButton &evt) override;
    virtual bool OnMouseUp(const dr4::Event::MouseButton &evt) override;
    virtual bool OnMouseMove(const dr4::Event::MouseMove &evt) override;

protected:
    bool is_drawing, is_selected;
    Canvas* canvas;
    MyShape* cur_shape;
    dr4::Window* window;
};

class TextTool : public MyTool {
public:
    TextTool();
    virtual MyShape* createShape() override;

    virtual std::string_view Icon() const override;
    virtual std::string_view Name() const override;

    virtual bool OnMouseDown(const dr4::Event::MouseButton &evt) override;
    virtual bool OnMouseUp(const dr4::Event::MouseButton &evt) override;
    virtual bool OnKeyDown(const dr4::Event::KeyEvent &evt) override;

protected:
    TextShape* cur_text_shape;
};

class RectTool : public MyTool {
public:
    RectTool();
    virtual MyShape* createShape() override;

    virtual std::string_view Icon() const override;
    virtual std::string_view Name() const override;
};

class CircleTool : public MyTool {
public:
    CircleTool();
    virtual MyShape* createShape() override;

    virtual std::string_view Icon() const override;
    virtual std::string_view Name() const override;
};

class LineTool : public MyTool {
public:
    LineTool();
    virtual MyShape* createShape() override;

    virtual std::string_view Icon() const override;
    virtual std::string_view Name() const override;
};

} // namespace pp

namespace cum {

class AbraCat_pp_plugin : public PPToolPlugin
{
public:
    AbraCat_pp_plugin();
    virtual std::vector<std::unique_ptr<pp::Tool>> CreateTools(pp::Canvas *cvs) override;

    virtual std::string_view GetIdentifier() const override;
    virtual std::string_view GetName() const override;
    virtual std::string_view GetDescription() const override;
    virtual std::vector<std::string_view> GetDependencies() const override;
    virtual std::vector<std::string_view> GetConflicts() const override;
    virtual void AfterLoad() override;

private:
    std::string name, description;
    // std::string_view descr_view;
};

} // namespace cum

extern "C" cum::Plugin *CreatePlugin();


#endif // ABRACAT_PP_PLUGIN_H