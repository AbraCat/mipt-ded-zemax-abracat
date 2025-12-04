#include "texture.h"
#include "sdl-adapter.h"
#include "assert.h"

#include "dr4/texture.hpp"
#include "dr4/math/color.hpp"

#include <cassert>

const int pix_bytes = 30, font_ptsize = 28;

static Vector colToMyVec(dr4::Color col) { return Vector(col.r, col.g, col.b); }
static Vector dr4ToMyVec(dr4::Vec2f vec) { return Vector(vec.x, vec.y, 0); }

namespace dr4 {

void dr4::MyLine::DrawOn(Texture& texture) const {
    MyTexture* my_t = dynamic_cast<MyTexture*>(&texture);
    assert(my_t != nullptr);
    SDL_SetRenderTarget(getRenderer(), my_t->t);

    Vec2f zero = texture.GetZero();

    setColor(colToMyVec(color));
    SDL_RenderLine(getRenderer(), start.x + zero.x, start.y + zero.y, end.x + zero.x, end.y + zero.y);
}

void dr4::MyLine::SetPos(Vec2f pos) {
    Vec2f diff = end - start;
    start = pos;
    end = pos + diff;
}

Vec2f dr4::MyLine::GetPos() const { return start; }

void dr4::MyLine::SetStart(Vec2f start) { this->start = start; }
void dr4::MyLine::SetEnd(Vec2f end) { this->end = end; }
void dr4::MyLine::SetColor(Color color) { this->color = color; }
void dr4::MyLine::SetThickness(float thickness) { this->thickness = thickness; }

dr4::Vec2f dr4::MyLine::GetStart() const { return start; }
dr4::Vec2f dr4::MyLine::GetEnd() const { return end; }
dr4::Color dr4::MyLine::GetColor() const { return color; }
float dr4::MyLine::GetThickness() const { return thickness; }




void dr4::MyCircle::DrawOn(Texture& texture) const {
    MyTexture* my_t = dynamic_cast<MyTexture*>(&texture);
    assert(my_t != nullptr);
    SDL_SetRenderTarget(getRenderer(), my_t->t);

    Vec2f zero = texture.GetZero();
    
    setColor(colToMyVec(fill_color));
    drawCircle(dr4ToMyVec(center + zero), radius.x, true);

    setColor(colToMyVec(border_color));
    drawCircle(dr4ToMyVec(center + zero), radius.x, false);
}

void dr4::MyCircle::SetPos(Vec2f pos) { center = pos; }
Vec2f dr4::MyCircle::GetPos() const { return center; }

void dr4::MyCircle::SetCenter(dr4::Vec2f center) { this->center = center; }
void dr4::MyCircle::SetRadius(Vec2f radius) { this->radius = radius; }
void dr4::MyCircle::SetFillColor(dr4::Color color) { this->fill_color = color; }
void dr4::MyCircle::SetBorderColor(dr4::Color color) { this->border_color = color; }
void dr4::MyCircle::SetBorderThickness(float thickness) { this->thickness = thickness; }

dr4::Vec2f dr4::MyCircle::GetCenter() const { return center; }
Vec2f dr4::MyCircle::GetRadius() const { return radius; }
dr4::Color dr4::MyCircle::GetFillColor() const { return fill_color; }
dr4::Color dr4::MyCircle::GetBorderColor() const { return border_color; }
float dr4::MyCircle::GetBorderThickness() const { return thickness; } 




void dr4::MyRectangle::DrawOn(Texture& texture) const {
    MyTexture* my_t = dynamic_cast<MyTexture*>(&texture);
    assert(my_t != nullptr);
    SDL_SetRenderTarget(getRenderer(), my_t->t);

    Vec2f zero = texture.GetZero();

    SDL_FRect r;
    r.x = pos.x + zero.x;
    r.y = pos.y + zero.y;
    r.w = size.x;
    r.h = size.y;
    
    setColor(colToMyVec(fill_color));
    SDL_RenderFillRect(getRenderer(), &r);

    setColor(colToMyVec(border_color));
    SDL_RenderRect(getRenderer(), &r);
}

void dr4::MyRectangle::SetPos(Vec2f pos) { this->pos = pos; }
Vec2f dr4::MyRectangle::GetPos() const { return pos; }

 void dr4::MyRectangle::SetSize(dr4::Vec2f size) { this->size = size; }
 void dr4::MyRectangle::SetFillColor(dr4::Color color) { this->fill_color = color; }
 void dr4::MyRectangle::SetBorderThickness(float thickness) { this->thickness = thickness; }
 void dr4::MyRectangle::SetBorderColor(dr4::Color color) { this->border_color = color; }

 dr4::Vec2f dr4::MyRectangle::GetSize() const { return size; }
 dr4::Color dr4::MyRectangle::GetFillColor() const { return fill_color; }
 float dr4::MyRectangle::GetBorderThickness() const { return thickness; }
 dr4::Color dr4::MyRectangle::GetBorderColor() const { return border_color; }



dr4::MyFont::MyFont() {
    ttf_font = nullptr;
}

void dr4::MyFont::LoadFromFile(const std::string &path) {
    ttf_font = TTF_OpenFont(path.c_str(), font_ptsize);
    if (!TTF_SetFontSize(ttf_font, font_ptsize)) return;
    if (ttf_font == nullptr) return;
    // setFont(font);
}

void dr4::MyFont::LoadFromBuffer(const void *buffer, size_t size) {}
float dr4::MyFont::GetAscent(float fontSize) const { return 0; }
float dr4::MyFont::GetDescent(float fontSize) const { return 0; }



dr4::MyText::MyText() {
    font = nullptr;
    ttf_font = nullptr;
}

void dr4::MyText::DrawOn(Texture& texture) const {
    MyTexture* my_t = dynamic_cast<MyTexture*>(&texture);
    assert(my_t != nullptr);

    Vec2f zero = texture.GetZero();

    SDL_SetRenderTarget(getRenderer(), my_t->t);
    setColor(colToMyVec(color));

    setFont(ttf_font);
    putText(text, dr4ToMyVec(pos + zero), dr4ToMyVec(pos + zero + GetBounds()), color);
}

void dr4::MyText::SetPos(Vec2f pos) { this->pos = pos; }
Vec2f dr4::MyText::GetPos() const { return pos; }

void dr4::MyText::SetText(const std::string &text) { this->text = text; }
void dr4::MyText::SetColor(dr4::Color color) { this->color = color; }
void dr4::MyText::SetVAlign(VAlign align) { this->v_align = align; }

void dr4::MyText::SetFontSize(float size) {
    this->font_size = size;
    TTF_SetFontSize(ttf_font, size);
}

void dr4::MyText::SetFont(const Font *font) {
    this->font = dynamic_cast<const MyFont*>(font);
    ttf_font = this->font->getTtfFont();
    TTF_SetFontSize(ttf_font, font_ptsize);
}

Vec2f dr4::MyText::GetBounds() const {
    int text_width = -1, text_height = -1;
    assert(TTF_GetStringSize(getFont(), text.c_str(), text.size(), &text_width, &text_height));
    return dr4::Vec2f(text_width + lft_text_pad, text_height);
}

const std::string &dr4::MyText::GetText() const { return text; }
Color              dr4::MyText::GetColor() const { return color; }
float              dr4::MyText::GetFontSize() const { return font_size; }
Text::VAlign             dr4::MyText::GetVAlign() const { return v_align; }
const Font        *dr4::MyText::GetFont() const { return font; }




void dr4::MyImage::DrawOn(Texture& texture) const {
    MyTexture* my_t = dynamic_cast<MyTexture*>(&texture);
    assert(my_t != nullptr);
    SDL_SetRenderTarget(getRenderer(), my_t->t);

    Vec2f zero = texture.GetZero();
    Vec2f texture_size = texture.GetSize();

    int x_upper = std::min(texture_size.x, pos.x + width);
    int y_upper = std::min(texture_size.y, pos.y + height);

    for (int x = pos.x; x < x_upper; ++x)
    {
        for (int y = pos.y; y < y_upper; ++y)
        {
            dr4::Color col = GetPixel(x - pos.x, y - pos.y);
            setColor(colToMyVec(col));

            SDL_RenderPoint(getRenderer(), x + zero.x, y + zero.y);
        }
    }
}

void dr4::MyImage::SetPos(Vec2f pos) { this->pos = pos; }
Vec2f dr4::MyImage::GetPos() const { return pos; }

dr4::MyImage::MyImage(unsigned w, unsigned h) : width(w), height(h), stride(w * pix_bytes)
{
    buffer = std::calloc(w * h, pix_bytes);
    assert(buffer != NULL);
    buf = (unsigned char*)buffer;

    pos = Vec2f(0, 0);
}

dr4::MyImage::~MyImage()
{
    free(buffer);
}

void dr4::MyImage::SetPixel(size_t x, size_t y, dr4::Color col)
{
    assert(x >= 0 && x < width && y >= 0 && y < height);

    buf[pix_bytes * (y * width + x) + 0] = (unsigned char)col.r;
    buf[pix_bytes * (y * width + x) + 1] = (unsigned char)col.g;
    buf[pix_bytes * (y * width + x) + 2] = (unsigned char)col.b;
}

dr4::Color dr4::MyImage::GetPixel(size_t x, size_t y) const
{
    assert(x >= 0 && x < width && y >= 0 && y < height);

    unsigned char r = buf[pix_bytes * (y * width + x) + 0],
    g = buf[pix_bytes * (y * width + x) + 1],
    b = buf[pix_bytes * (y * width + x) + 2];

    return dr4::Color(r, g, b, 255);
}


void dr4::MyImage::SetSize(Vec2f size)
{
    this->width = size.x;
    this->height = size.y;

    free(buffer);

    buffer = std::calloc(width * height, pix_bytes);
    assert(buffer != NULL);
    buf = (unsigned char*)buffer;
}


dr4::Vec2f dr4::MyImage::GetSize() const { return dr4::Vec2f(width, height); }
float dr4::MyImage::GetWidth() const { return width; }
float dr4::MyImage::GetHeight() const { return height; }




dr4::MyTexture::MyTexture(dr4::Vec2f size) : w(size.x), h(size.y)
{
    t = SDL_CreateTexture(getRenderer(), SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_TARGET, w, h);
    pos = zero = Vec2f();
}

dr4::MyTexture::~MyTexture() { SDL_DestroyTexture(t); }

void dr4::MyTexture::Clear(dr4::Color color)
{
    SDL_SetRenderTarget(getRenderer(), t);
    setColor(Vector(color.r, color.g, color.b));
    SDL_RenderClear(getRenderer());
}

dr4::Image* dr4::MyTexture::GetImage() const { return nullptr; }

void dr4::MyTexture::SetSize(Vec2f size) {
    SDL_DestroyTexture(t);

    w = size.x;
    h = size.y;
    t = SDL_CreateTexture(getRenderer(), SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_TARGET, w, h);
}

dr4::Vec2f dr4::MyTexture::GetSize() const { return Vec2f(w, h); }
float dr4::MyTexture::GetWidth() const { return w; }
float dr4::MyTexture::GetHeight() const { return h; }

void dr4::MyTexture::SetPos(Vec2f pos) { this->pos = pos; }
Vec2f dr4::MyTexture::GetPos() const { return pos; }

void dr4::MyTexture::SetZero(Vec2f pos) { this->zero = pos; }
Vec2f dr4::MyTexture::GetZero() const { return zero; }

void MyTexture::SetClipRect(Rect2f rect) { clip_rect = rect; }
void MyTexture::RemoveClipRect() { clip_rect = Rect2f(); }
Rect2f MyTexture::GetClipRect() const { return clip_rect; }

void dr4::MyTexture::DrawOn(Texture& texture) const
{
    const MyTexture* sdl_t = dynamic_cast<const MyTexture*>(&texture);
    assert(sdl_t != nullptr);
    Vec2f zero = texture.GetZero();

    SDL_SetRenderTarget(getRenderer(), sdl_t->t);

    SDL_FRect dst_rect;
    dst_rect.x = pos.x + zero.x;
    dst_rect.y = pos.y + zero.y;
    dst_rect.w = w;
    dst_rect.h = h;
    SDL_RenderTexture(getRenderer(), t, NULL, &dst_rect);
}

} // namespace dr4





CoordSystem::CoordSystem(Vector centre, double xScale, double yScale) :
    centre(centre), xScale(xScale), yScale(yScale)
{
    //
}

Vector CoordSystem::getAbsCentre() { return centre; }

Vector CoordSystem::tranformToBaseCoord(Vector coord)
{
    Vector objectCoord, absCentre = getAbsCentre();
    objectCoord.x = coord.x * xScale + absCentre.x;
    objectCoord.y = coord.y * yScale + absCentre.y;

    return objectCoord;
}

Vector CoordSystem::baseToTransformCoord(Vector coord)
{
    Vector planeCoord, absCentre = getAbsCentre();
    planeCoord.x = (coord.x - absCentre.x) * 1.0 / xScale;
    planeCoord.y = (coord.y - absCentre.y) * 1.0 / yScale;

    return planeCoord;
}

void CoordSystem::transform(Vector centre, double xScale, double yScale)
{
    this->centre = centre;
    this->xScale = xScale;
    this->yScale = yScale;
}

void CoordSystem::move(Vector newPos) { transform(newPos, xScale, yScale); }
void CoordSystem::rescale(double new_scale_x, double new_scale_y) { transform(centre, new_scale_x, new_scale_y); }
void CoordSystem::rescale(double new_scale_x, double new_scale_y, Vector point)
{
    Vector pos_change = point - centre;
    move(point);

    double scale_change_x = new_scale_x / xScale, scale_change_y = new_scale_y / yScale;
    rescale(new_scale_x, new_scale_y);

    pos_change.x *= scale_change_x;
    pos_change.y *= scale_change_y;
    move(point - pos_change);
}




// Texture::Texture(Widget* w) : CoordSystem({0, 0}, 1, 1)
// {
//     this->updated = 1;
//     this->w = w;
//     this->visible_in = w;
// }

// void Texture::setVisibleIn(Widget* w) { this->visible_in = w; }

// Vector Texture::getAbsCentre() { return centre + w->getAbsTL(); }

// void Texture::rescaleCentre(double new_scale_x, double new_scale_y)
// {
//     rescale(new_scale_x, new_scale_y, w->wh * 0.5);
// }

// void Texture::render()
// {   
//     if (w->w_border_visible)
//     {
//         setColor(whiteV);
//         drawRect(w->absTL, w->absTL + w->wh, 0);
//     }

//     for (ColFixedVec rect: rects) paintRect(rect);
//     for (ColPoint p : points) paintPoint(p);
//     for (ColFixedVec l: lines) paintLine(l);
//     for (ColCircle c: circles) paintCircle(c);
//     for (ColPolygon pol: polygons) paintPolygon(pol);
//     paintText();

//     updated {}
// }

// void Texture::paintText()
// {
//     const double text_h = 10;
//     if (text.size() != 0 && visible_in->inAbsRect(w->getAbsTL() + w->wh * 0.5 + Vector(0, text_h)) &&
//                             visible_in->inAbsRect(w->getAbsTL() + w->wh * 0.5 + Vector(0, -text_h)))
//         putText(text, w->getAbsTL(), w->getAbsTL() + w->wh);
// }

// void Texture::renderRec()
// {
//     render();
//     for (Widget* child: w->children)
//         child->t->renderRec();
// }

// void Texture::renderIfUpdatedRec()
// {
//     if (updated) renderRec();

//     for (Widget* child: w->children)
//         child->t->renderIfUpdatedRec();

//     updated {}
// }

// void Texture::clear()
// {
//     points.clear();
//     lines.clear();
//     rects.clear();
//     circles.clear();
//     polygons.clear();

//     updated = 1;
// }

// void Texture::paintPoint(ColPoint p)
// {
//     Vector absP = tranformToBaseCoord(p.p);
//     if (visible_in->inAbsRect(absP))
//     {
//         setColor(p.col);
//         drawPoint(absP);
//     }
// }

// void Texture::paintLine(ColFixedVec l)
// {
//     Vector absP1 = tranformToBaseCoord(l.vec.p1), absP2 = tranformToBaseCoord(l.vec.p2);
//     FixedVec ans;

//     if (clipLine({absP1, absP2}, {visible_in->absTL, visible_in->absTL + visible_in->wh}, &ans));
//     {
//         setColor(l.col);
//         drawLine(ans.p1, ans.p2);
//     }
// }

// void Texture::paintRect(ColFixedVec rect)
// {
//     if (rect.fill == 0) assert("should pass rect border as 4 lines" && 0);

//     Vector absP1 = tranformToBaseCoord(rect.vec.p1), absP2 = tranformToBaseCoord(rect.vec.p2);

//     FixedVec intersection;
//     if (rectIntersection({absP1, absP2},
//         {visible_in->absTL, visible_in->absTL + visible_in->wh}, &intersection))
//     {
//         setColor(rect.col);
//         drawRect(intersection.p1, intersection.p2, rect.fill);
//     }

//     // setColor(rect.col);
//     // drawRect(absP1, absP2, rect.fill);
// }

// void Texture::paintCircle(ColCircle c)
// {
//     Vector absC = tranformToBaseCoord(c.centre);
//     if (visible_in->inAbsRect(absC))
//     {
//         setColor(c.col);
//         drawCircle(absC, c.r * xScale, c.fill);
//     }
// }

// void Texture::paintPolygon(ColPolygon pol)
// {
//     if (pol.fill == 0) assert("polygon border unimplemented" && 0);

//     std::vector<Vector> absPoints;
//     for (Vector p: pol.ps)
//         absPoints.push_back(tranformToBaseCoord(p));

//     setColor(pol.col);
//     fillConvexPolygon(absPoints);
// }

// void Texture::addText(std::string text)
// {
//     this->text = text;
//     updated = 1;
//     state->needs_rerender = 1;
// }

// void Texture::addPoint(Vector p, Vector color)
// {
//     points.push_back({p, color});
//     updated = 1;
//     state->needs_rerender = 1;
// }

// void Texture::addLine(FixedVec line, Vector color)
// {
//     lines.push_back({line, color, 0});
//     updated = 1;
//     state->needs_rerender = 1;
// }

// void Texture::addRect(FixedVec rect, Vector color, bool fill)
// {
//     if (fill) rects.push_back({rect, color, fill});
//     else
//     {
//         Vector bl(rect.p1.x, rect.p2.y), tr(rect.p2.x, rect.p1.y);

//         addLine({rect.p1, bl}, color);
//         addLine({rect.p1, tr}, color);
//         addLine({rect.p2, bl}, color);
//         addLine({rect.p2, tr}, color);
//     }

//     updated = 1;
//     state->needs_rerender = 1;
// }

// void Texture::addCircle(Vector centre, Vector col, double r, bool fill)
// {
//     circles.push_back({centre, col, r, fill});
//     updated = 1;
//     state->needs_rerender = 1;
// }

// void Texture::addPolygon(std::vector<Vector> points, Vector color, bool fill)
// {
//     polygons.push_back({points, color, fill});
//     updated = 1;
//     state->needs_rerender = 1;
// }

// void Texture::addVector(FixedVec v, Vector color)
// {
//     const double arrowCoeff = 0.2;

//     Vector vec = fixedToFree(v);
//     Vector ort1 = {vec.y, -vec.x}, ort2 = {-vec.y, vec.x};
//     Vector e1 = (ort1 - vec) * arrowCoeff, e2 = (ort2 - vec) * arrowCoeff;

//     addLine(v, color);
//     addPolygon({v.p2, v.p2 + e2, v.p2 + e1}, color, 1);

//     updated = 1;
//     state->needs_rerender = 1;
// }




// PixelTexture::PixelTexture(Widget* w) : Texture(w)
// {
//     pix = new Vector[w->width * w->height];
// }

// PixelTexture::~PixelTexture() { delete pix; }

// void PixelTexture::render()
// {
//     for (int y {} y < w->height; ++y)
//     {
//         for (int x {} x < w->width; ++x)
//         {
//             setColor(getPix(x, y));
//             drawPoint(Vector(x, y) + w->getAbsTL());
//         }
//     }

//     Texture::render();
// }

// void PixelTexture::setPix(int x, int y, Vector col)
// {
//     pix[y * w->width + x] = col;
//     updated = 1;
//     state->needs_rerender = 1;
// }

// Vector PixelTexture::getPix(int x, int y) { return pix[y * w->width + x]; }
