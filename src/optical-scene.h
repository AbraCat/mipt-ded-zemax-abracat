#ifndef OPTICAL_SCENE_H
#define OPTICAL_SCENE_H

#include <set>

#include "optical-object.h"
#include "mycanvas.h"
// #include "wcontainer.h"
// #include "plane.h"
// #include "button.h"
// #include "scroll.h"

#include "hui/widget.hpp"

class OptController;

using SurfaceIt = std::vector<Surface*>::iterator;
using SourceIt = std::vector<Source*>::iterator;

extern const double ratio;

// Vector getDiffuseColor(Surface* s, Source* l, Vector p_surface, Vector p_light);

class OptScene : public hui::Widget
{
public:
    OptScene(hui::UI* state, hui::Widget* parent, dr4::Vec2f pos, dr4::Vec2f size);
    virtual hui::EventResult OnIdle(hui::IdleEvent &evt) override;
    void needsRerender();

    dr4::Texture* getTexture() const;
    FixedVec getRect(OptObject* obj) const;

    Vector screen_to_pixels(Vector p) const;
    Vector pixels_to_screen(IntVec pix) const;

    void setV(Vector V);
    void moveCamera(Vector change);
    void attachCanvas(pp::MyCanvas* cvs);

    Vector traceRay(Ray ray, int depth) const;
    Surface* getIntersectedSurface(Ray ray, double *t_ptr) const;
    Vector castShadowRay(Surface* s, Source* l, Vector p) const;
    Vector traceDiffuse(Surface* s, Vector p) const;
    Vector traceRefract(Surface* s, Ray ray, Vector p, int depth) const;

protected:
    virtual void Redraw() const override;

public:
// private:
    bool redraw_picture;
    Vector V, screen_tl, screen_w, screen_h;

    std::vector<Surface*> surfaces;
    std::vector<Source*> sources;
    std::set<OptObject*> selected;

    int size_x, size_y;

    OptController* control;
    mutable bool needs_rerender;
    dr4::Image* img;
    mutable std::vector<IntVec> pix_queue;
    pp::MyCanvas* canvas;
    // PixelTexture *pix_texture;
};

#endif // OPTICAL_SCENE_H