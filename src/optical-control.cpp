#include "optical-control.h"
#include "scroll.h"
#include "colors.h"

#include <algorithm>
#include <random>
#include <sstream>

#include <limits>
#include <cmath>
#include <cassert>

const Vector sky_col = {0, 0.5, 0.75}, init_V = {0, 0, 10}, init_screen_tl = {-2, -1.15, 4};
const Vector std_sphere_pos = Vector(0, 0, 0), std_sphere_col = Vector(0.5, 0.5, 0.5), std_src_pos = Vector(0, -1, 0),
    std_src_col = Vector(0.5, 0.5, 0.5);

extern const int scene_w = 1000;
const double cam_change_x = 0.5, cam_change_y = 0.5, cam_change_z = 1, obj_change = 1,
    std_sphere_radius = 0.5, std_src_radius = 0.5, property_name_portion = 0.55;
const int scene_h = scene_w / ratio, button_h = 50, obj_list_w = 150,
    obj_button_h = obj_list_w / 1.4, properties_h = scene_h * 0.7, obj_scroll_w = 50, 
    properties_w = 500, properties_left = scene_w + obj_list_w + obj_scroll_w,
    n_camera_buttons = 6, n_move_buttons = 6, max_n_objects = 10;

extern const int opt_control_w = scene_w + obj_list_w + obj_scroll_w + properties_w,
    opt_control_h = scene_h + button_h, tools_h = opt_control_h * 0.7, tool_button_w = 200;


std::string doubleToStr(double val)
{
    std::ostringstream out;
    out.precision(2);
    out << std::fixed << val;
    return std::move(out).str();
}

ObjControlPanel::ObjControlPanel(hui::UI *ui, dr4::Vec2f pos, dr4::Vec2f size)
    : MyContainer(ui, pos, size)
{
    // setFillRect(1);
    
    prop_cont = new WContainer(ui, {0, 0}, {GetSize().x, properties_h}, OPT_TOTAL, 1);
    button_cont = new WContainer(ui, {0, properties_h}, {GetSize().x, GetSize().y - properties_h}, n_move_buttons + 1, 1);
    addChild(prop_cont);
    addChild(button_cont);
}

void ObjControlPanel::setObject(OptObject* obj)
{
    prop_cont->clearChildren();
    button_cont->clearChildren();

    if (obj != nullptr)
    {
        for (OptProperty prop: obj->getProperties()) {
            prop_cont->addChild(new OptPropWidget(GetUI(), {}, prop_cont->getChildSize(), obj, prop));
        }

        dr4::Vec2f button_size = button_cont->getChildSize();
        button_cont->addChild(new MoveObjectButton(GetUI(), {}, button_size, obj, {0, 0, -obj_change}, "Forward", this));
        button_cont->addChild(new MoveObjectButton(GetUI(), {}, button_size, obj, {0, 0, obj_change}, "Back", this));
        button_cont->addChild(new MoveObjectButton(GetUI(), {}, button_size, obj, {-obj_change, 0, 0}, "Left", this));
        button_cont->addChild(new MoveObjectButton(GetUI(), {}, button_size, obj, {obj_change, 0, 0}, "Right", this));
        button_cont->addChild(new MoveObjectButton(GetUI(), {}, button_size, obj, {0, -obj_change, 0}, "Up", this));
        button_cont->addChild(new MoveObjectButton(GetUI(), {}, button_size, obj, {0, obj_change, 0}, "Down", this));

        button_cont->addChild(new DeleteObjectButton(GetUI(), {}, button_size, obj, "Delete"));
    }

    prop_cont->ForceRedraw();
    button_cont->ForceRedraw();
}

void ObjControlPanel::setDisplayedVal(OptPropEnum prop, double val)
{
    for (Widget* w: prop_cont->children)
    {
        OptPropWidget* prop_w = dynamic_cast<OptPropWidget*>(w);
        assert(prop_w != nullptr);

        if (prop_w->val_field->prop.prop == prop)
        {
            prop_w->val_field->SetText(doubleToStr(val));
            break;
        }
    }
}

OptPropWidget::OptPropWidget(hui::UI *ui, dr4::Vec2f pos, dr4::Vec2f size, OptObject* obj, OptProperty prop)
    : MyContainer(ui, pos, size)
{
    name_field = new TextField(ui, {0, 0}, {GetSize().x * property_name_portion, GetSize().y});
    name_field->SetText(prop.getName());
    addChild(name_field);

    val_field = new OptPropField(this, ui, {GetSize().x * property_name_portion, 0},
        {GetSize().x * (1 - property_name_portion), GetSize().y}, obj, prop);
    addChild(val_field);
}

OptPropField::OptPropField(OptPropWidget* parent, hui::UI *ui, dr4::Vec2f pos, dr4::Vec2f size, OptObject* obj, OptProperty prop)
    : InputField(ui, pos, size, black_color, doubleToStr(prop.val)), obj(obj), prop(prop)
{
    setValidator([](std::string s){
        try
        {
            double val = std::stod(s);
            return true;
        }
        catch(std::exception& e)
        {
            return false;
        }
    });
}

void OptPropField::action()
{
    obj->setProperty(prop.prop, std::stod(getText()));
    obj->scene->needsRerender();
}

OptObjectButton::OptObjectButton(hui::UI *ui, dr4::Vec2f pos, dr4::Vec2f size, OptObject* obj, ObjControlPanel* panel)
    : ToggleButton(ui, pos, size, gray_color, obj->name), obj(obj)
{
    //
}

void OptObjectButton::action() { obj->scene->control->select(obj); }
void OptObjectButton::deactivate() { obj->scene->control->deselect(obj); }



MoveObjectButton::MoveObjectButton(hui::UI *ui, dr4::Vec2f pos, dr4::Vec2f size, OptObject* obj,
    Vector change, std::string text, ObjControlPanel* panel)
    : Button(ui, pos, size, gray_color, text), obj(obj), change(change), panel(panel)
{
    //
}

void MoveObjectButton::action()
{
    obj->movePos(change);

    panel->setDisplayedVal(OPT_POS_X, obj->pos.x);
    panel->setDisplayedVal(OPT_POS_Y, obj->pos.y);
    panel->setDisplayedVal(OPT_POS_Z, obj->pos.z);

    obj->scene->needsRerender();
}

DeleteObjectButton::DeleteObjectButton(hui::UI *ui, dr4::Vec2f pos, dr4::Vec2f size, OptObject* obj, std::string text)
    : Button(ui, pos, size, red_color, text), obj(obj)
{
    //
}

void DeleteObjectButton::action()
{
    obj->scene->control->deleteObject(obj);
}

AddObjectButton::AddObjectButton(hui::UI *state, dr4::Vec2f pos, dr4::Vec2f size, OptObjectType type,
    std::string text, OptController* control)
    : Button(state, pos, size, light_green_color, text), control(control)
{
    this->type = type;
}

void AddObjectButton::action() {
    OptObject* obj = nullptr;
    switch (type) {
        case OPT_OBJ_SHPERE:
            obj = *control->addSphere(std_sphere_pos, std_sphere_col, std_sphere_radius);
            break;
        case OPT_OBJ_SOURCE:
            obj = *control->addSource(std_src_pos, std_src_col, std_src_radius);
            break;
    }
    if (obj == nullptr) return;

    for (hui::Widget* w: control->obj_cont->children) {
        OptObjectButton* button = dynamic_cast<OptObjectButton*>(w);
        assert(button != nullptr);

        if (button->obj == obj || button->obj != obj && button->isPressed()) button->imitatePress(true);
    }
    control->s->needsRerender();
    control->obj_scroll->moveThumb(0);
}


MoveCameraButton::MoveCameraButton(hui::UI* ui, OptScene* scene, Vector change, dr4::Color color, std::string text)
    : Button(ui, dr4::Vec2f(), dr4::Vec2f(), color, text), scene(scene), change(change)
{
    //
}

void MoveCameraButton::action()
{
    scene->moveCamera(change);
    scene->needsRerender();
}


OptController::OptController(hui::UI* ui, MyContainer* parent) : parent(parent)
{
    s = new OptScene(ui, nullptr, dr4::Vec2f(0, 0), dr4::Vec2f(scene_w, scene_w / ratio), this);
    scene_cvs_widget = new CanvasWidget(ui, {0, 0}, s);
    parent->addChild(scene_cvs_widget);

    panel = new ObjControlPanel(ui, {properties_left, 0},
        {properties_w, scene_h + button_h});
    parent->addChild(panel);

    cam_cont = new WContainer(ui, {0, scene_h}, {scene_w, button_h}, n_camera_buttons, 0);
    parent->addChild(cam_cont);
    cam_cont->addChild(new MoveCameraButton(ui, s, {0, 0, -cam_change_z}, gray_color, "Forward"));
    cam_cont->addChild(new MoveCameraButton(ui, s, {0, 0, cam_change_z}, gray_color, "Back"));
    cam_cont->addChild(new MoveCameraButton(ui, s, {-cam_change_x, 0, 0}, gray_color, "Left"));
    cam_cont->addChild(new MoveCameraButton(ui, s, {cam_change_x, 0, 0}, gray_color, "Right"));
    cam_cont->addChild(new MoveCameraButton(ui, s, {0, -cam_change_y, 0}, gray_color, "Up"));
    cam_cont->addChild(new MoveCameraButton(ui, s, {0, cam_change_y, 0}, gray_color, "Down"));

    WContainer* add_but_cont = new WContainer(ui, {opt_control_w, tools_h}, {tool_button_w, opt_control_h - tools_h}, 2, true);
    add_but_cont->addChild(new AddObjectButton(ui, {}, {}, OPT_OBJ_SHPERE, "Add sphere", this));
    add_but_cont->addChild(new AddObjectButton(ui, {}, {}, OPT_OBJ_SOURCE, "Add source", this));
    parent->addChild(add_but_cont);

    obj_cont = makeObjectContainer({0, 0}, {obj_list_w, obj_button_h});
    assert(obj_cont != nullptr);
    ScrollableWidget* obj_scrollable = new ScrollableWidget(obj_cont, {scene_w, 0}, {obj_list_w, scene_h + button_h});
    parent->addChild(obj_scrollable);

    obj_scroll = new WidgetScrollBar(ui, {scene_w + obj_list_w, 0},
        {obj_scroll_w, scene_h + button_h}, obj_scrollable);
    parent->addChild(obj_scroll);
}

WList* OptController::makeObjectContainer(dr4::Vec2f pos, dr4::Vec2f size)
{
    int n_objects = s->surfaces.size() + s->sources.size();
    this->obj_cont = new WList(parent->GetUI(), pos, true, size.y, size.x);
    
    for (OptObject* obj: s->surfaces)
        obj_cont->addChild(new OptObjectButton(s->GetUI(), {}, {}, obj, panel));
    for (OptObject* obj: s->sources)
        obj_cont->addChild(new OptObjectButton(s->GetUI(), {}, {}, obj, panel));

    return obj_cont;
}

void OptController::select(OptObject* obj)
{
    s->selected.insert(obj);
    selected_changed();
}

void OptController::deselect(OptObject* obj)
{
    s->selected.erase(obj);
    selected_changed();
}

void OptController::selected_changed()
{
    if (s->selected.size() == 1)
        panel->setObject(*(s->selected).begin());
    else
        panel->setObject(nullptr);

    s->ForceRedraw();
}

void OptController::deleteObject(OptObject* obj)
{
    for (std::vector<Surface*>::iterator it = s->surfaces.begin(); it != s->surfaces.end(); ++it)
    {
        if (*it == obj)
        {
            s->surfaces.erase(it);
            break;
        }
    }

    for (std::vector<Source*>::iterator it = s->sources.begin(); it != s->sources.end(); ++it)
    {
        if (*it == obj)
        {
            s->sources.erase(it);
            break;
        }
    }

    s->selected.erase(obj);
    panel->setObject(nullptr);

    obj_cont->removeChildByPredicate([obj](hui::Widget* w){
        OptObjectButton* button = dynamic_cast<OptObjectButton*>(w);
        assert(button != nullptr);
        return button->obj == obj;
    });
    obj_scroll->moveThumb(0);

    delete obj;
    obj_cont->ForceRedraw();
    s->needsRerender();
}

void OptController::addObject(OptObject* obj)
{
    OptObjectButton* button = new OptObjectButton(s->GetUI(), {}, dr4::Vec2f(100, 100), obj, panel);
    obj_cont->addChild(button);
    obj_cont->ForceRedraw();
}

std::vector<Surface*>::iterator OptController::addSphere(Vector pos, Vector color, double r, Material m)
{
    std::string name = "Sphere " + std::to_string(s->surfaces.size());
    SphereSurface* new_sphere = new SphereSurface(pos, r, color, name, s, m);

    s->surfaces.push_back(new_sphere);
    addObject(new_sphere);
    return s->surfaces.end() - 1;
}

std::vector<Source*>::iterator OptController::addSource(Vector pos, Vector color, double r)
{
    std::string name = "Source " + std::to_string(s->sources.size());
    Source* new_source = new SphereSource(color, pos, r, name, s);

    s->sources.push_back(new_source);
    addObject(new_source);
    return s->sources.end() - 1;
}

