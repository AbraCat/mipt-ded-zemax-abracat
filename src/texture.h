#ifndef MY_TEXTURE_H
#define MY_TEXTURE_H

#include "SDL3/SDL.h"

#include "myvector.h"

#include "dr4/texture.hpp"

#include <vector>
#include <string>

namespace dr4
{

class MyLine: public Line {
private:
    Vec2f start, end;
    Color color;
    float thickness;

public:

    virtual void DrawOn(Texture& texture) const override;
    virtual void SetPos(Vec2f pos) override;
    virtual Vec2f GetPos() const override;

    virtual void SetStart(Vec2f start) override;
    virtual void SetEnd(Vec2f end) override;
    virtual void SetColor(Color color) override;
    virtual void SetThickness(float thickness) override;

    virtual Vec2f GetStart() const override;
    virtual Vec2f GetEnd() const override;
    virtual Color GetColor() const override;
    virtual float GetThickness() const override;
};

class MyCircle: public Circle {
private:
    Vec2f center, radius;
    Color border_color, fill_color;
    float thickness;

public:

    virtual void DrawOn(Texture& texture) const override;
    virtual void SetPos(Vec2f pos) override;
    virtual Vec2f GetPos() const override;

    virtual void SetCenter(Vec2f center) override;
    virtual void SetRadius(Vec2f radius) override;
    virtual void SetFillColor(Color color) override;
    virtual void SetBorderColor(Color color) override;
    virtual void SetBorderThickness(float thickness) override;
    
    virtual Vec2f GetCenter() const override;
    virtual Vec2f GetRadius() const override;
    virtual Color GetFillColor() const override;
    virtual Color GetBorderColor() const override;
    virtual float GetBorderThickness() const override;
};

class MyRectangle: public Rectangle {
private:
    Vec2f size, pos;
    Color fill_color, border_color;
    float thickness;

public:

    virtual void DrawOn(Texture& texture) const override;
    virtual void SetPos(Vec2f pos) override;
    virtual Vec2f GetPos() const override;

    virtual void SetSize(Vec2f size) override;
    virtual void SetFillColor(Color color) override;
    virtual void SetBorderThickness(float thickness) override;
    virtual void SetBorderColor(Color color) override;

    virtual Vec2f GetSize() const override;
    virtual Color GetFillColor() const override;
    virtual float GetBorderThickness() const override;
    virtual Color GetBorderColor() const override;
};

class MyFont : public Font {

public:
    virtual void LoadFromFile(const std::string &path) override;
    virtual void LoadFromBuffer(const void *buffer, size_t size) override;

    virtual float GetAscent(float fontSize) const override;
    virtual float GetDescent(float fontSize) const override;
};
    
class MyText: public Text {
private:
    std::string text;
    Color color;
    float font_size;
    VAlign v_align;
    const Font* font;
    Vec2f pos;

public:
    virtual void DrawOn(Texture& texture) const override;
    virtual void SetPos(Vec2f pos) override;
    virtual Vec2f GetPos() const override;

    virtual void SetText(const std::string &text) override;
    virtual void SetColor(Color color) override;
    virtual void SetFontSize(float size) override;
    virtual void SetVAlign(VAlign align) override;
    virtual void SetFont(const Font *font) override;

    virtual Vec2f              GetBounds() const override;
    virtual const std::string &GetText() const override;
    virtual Color              GetColor() const override;
    virtual float              GetFontSize() const override;
    virtual VAlign             GetVAlign() const override;
    virtual const Font        *GetFont() const override;
};

class MyImage : public Image
{
    void *buffer;
    unsigned char *buf;
    // PixelFormat format; // RGBA32, ...
    Vec2f pos;
    size_t width, height;
    size_t stride; // количество байт на один ряд

public:
    virtual void DrawOn(Texture& texture) const override;
    virtual void SetPos(Vec2f pos) override;
    virtual Vec2f GetPos() const override;

    MyImage(unsigned w, unsigned h);
    ~MyImage();

    virtual void SetPixel(size_t x, size_t y, dr4::Color col) override;
    virtual dr4::Color GetPixel(size_t x, size_t y) const override;

    virtual void SetSize(Vec2f size) override;
    virtual Vec2f GetSize() const override;
    virtual float GetWidth() const override;
    virtual float GetHeight() const override;
};

class MyTexture : public Texture
{
public:
    MyTexture(dr4::Vec2f size);
    ~MyTexture();

    virtual void Clear(dr4::Color color) override;

    virtual void SetSize(Vec2f size) override;
    virtual Vec2f GetSize() const override;
    virtual float GetWidth() const override;
    virtual float GetHeight() const override;

    virtual void DrawOn(Texture& texture) const override;
    virtual void SetPos(Vec2f pos) override;
    virtual Vec2f GetPos() const override;

    virtual void SetZero(Vec2f pos) override;
    virtual Vec2f GetZero() const override;

    virtual void SetClipRect(Rect2f rect) override;
    virtual void RemoveClipRect() override;
    virtual Rect2f GetClipRect() const override;

    int w, h;
    Vec2f pos, zero;
    Rect2f clip_rect; // TODO
    SDL_Texture *t;
};

}









class Widget;


class CoordSystem
{
public:
    CoordSystem(Vector centre, double xScale, double yScale);

    Vector tranformToBaseCoord(Vector coord);
    Vector baseToTransformCoord(Vector coord);

    virtual void transform(Vector centre, double xScale, double yScale);
    void move(Vector newPos);
    void rescale(double new_scale_x, double new_scale_y);
    void rescale(double new_scale_x, double new_scale_y, Vector point);

    virtual Vector getAbsCentre();

// protected:
    Vector centre;
    double xScale, yScale;
};



struct ColPoint
{
    Vector p, col;
};

struct ColFixedVec
{
    FixedVec vec;
    Vector col;
    bool fill;
};

struct ColCircle
{
    Vector centre, col;
    double r;
    bool fill;
};

struct ColPolygon
{
    std::vector<Vector> ps;
    Vector col;
    bool fill;
};

class Texture : public CoordSystem
{
public:
    Texture(Widget* w);
    virtual void render();
    void renderRec();
    void renderIfUpdatedRec();
    void clear();

    void setVisibleIn(Widget* w);

    virtual Vector getAbsCentre() override;
    void rescaleCentre(double new_scale_x, double new_scale_y);

    void addText(std::string text);
    virtual void addPoint(Vector p, Vector color);
    void addLine(FixedVec line, Vector color);

    void addRect(FixedVec rect, Vector color, bool fill);
    void addCircle(Vector centre, Vector col, double r, bool fill);
    void addPolygon(std::vector<Vector> points, Vector color, bool fill);
    void addVector(FixedVec v, Vector color);

    void paintPoint(ColPoint p);
    void paintLine(ColFixedVec line);
    void paintRect(ColFixedVec rect);
    void paintCircle(ColCircle cirlce);
    void paintPolygon(ColPolygon polygon);
    void paintText();

// protected:
    bool updated;
    Widget *w, *visible_in;

    std::string text;
    std::vector<ColPoint> points;
    std::vector<ColFixedVec> lines, rects;
    std::vector<ColCircle> circles;
    std::vector<ColPolygon> polygons;
};

class PixelTexture : public Texture
{
public:
    PixelTexture(Widget* w);
    ~PixelTexture();
    virtual void render() override;

    void setPix(int x, int y, Vector col);
    Vector getPix(int x, int y);

    Vector* pix;
};

#endif // MY_TEXTURE_H