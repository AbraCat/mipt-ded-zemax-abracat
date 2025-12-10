#include "optical-control.h"
#include "scroll.h"
#include "colors.h"
#include "my-utils.h"
#include "hui/ui.hpp"

#include <algorithm>
#include <random>
#include <fstream>
#include <iostream>

#include <limits>
#include <cmath>
#include <cassert>

const std::string scene_data_path = "opt-scene-data.txt";
const Vector sky_col = {0, 0.5, 0.75}, init_V = {0, 0, 10}, init_screen_tl = {-2, -1.15, 4};
const Vector std_sphere_pos = Vector(0, 0, 0), std_sphere_col = Vector(0.5, 0.5, 0.5), std_src_pos = Vector(0, -1, 0),
    std_src_col = Vector(0.5, 0.5, 0.5);

extern const int menu_w, menu_h;
extern const int scene_w = 1400;
const double prop_increase_step = 0.1, cam_change_x = 0.5, cam_change_y = 0.5, cam_change_z = 1, obj_change = 1,
    std_sphere_radius = 0.5, std_src_radius = 0.5, property_name_portion = 0.5 / 0.75;
  
const int scene_h = scene_w / ratio, button_h = 50, obj_list_w = 450,
    obj_button_h = obj_list_w / 4, obj_scroll_w = 20, obj_cont_h = 300,
    properties_w = 500, properties_left = scene_w + obj_list_w + obj_scroll_w;

// number of buttons
const int n_camera_buttons = 6, n_mov_but_x = 3, n_mov_but_y = 2, n_move_buttons = n_mov_but_x * n_mov_but_y;

// object control panel sizes
const int panel_button_h = (scene_h + button_h) / (1 + OPT_TOTAL + n_mov_but_y + 1) + 1,
    properties_h = panel_button_h * OPT_TOTAL, obj_move_cont_h = panel_button_h * n_mov_but_y, n_save_buttons = 2;

extern const int opt_control_w = scene_w + obj_list_w + obj_scroll_w + properties_w,
    opt_control_h = scene_h + button_h, tools_h = 100 * 4, tool_button_w = 160;



ObjControlPanel::ObjControlPanel(hui::UI *ui, dr4::Vec2f pos, dr4::Vec2f size, OptController* control)
    : MyContainer(ui, pos, size), control(control), obj(nullptr)
{
    const double move_but_portion = 0.25, name_portion = 0.5, field_portion = 1 - move_but_portion - name_portion;
    const int panel_button_h = 30;

    name_text = new TextField(ui, {size.x * move_but_portion, 0}, {size.x * name_portion, GetSize().y * 0.1});
    name_field = new OptNameField(ui, {size.x * (move_but_portion + name_portion), 0},
        {size.x * field_portion, GetSize().y * 0.1}, control);
    name_field->setMaxTextLen(obj_list_w);
    name_text->setRectPadding(5);
    name_field->setRectPadding(5);

    prop_cont = new WContainer(ui, {GetSize().x * move_but_portion, size.y * 0.1},
        {GetSize().x * (name_portion + field_portion), GetSize().y * 0.9}, OPT_TOTAL, 1);
    button_cont = new WContainer(ui, {0, 0}, {GetSize().x * move_but_portion, size.y}, 6 + 1, 1);
    prop_cont->setDrawRect(false);

    const int delete_button_y = button_cont->GetPos().y + button_cont->GetSize().y;
    delete_button = nullptr;

    addChild(name_text);
    addChild(name_field);
    addChild(prop_cont);
    addChild(button_cont);
}

void ObjControlPanel::Redraw() const {
    if (obj != nullptr) {
        MyContainer::Redraw();
        return;
    }

    dr4::Rectangle* rect = GetUI()->GetWindow()->CreateRectangle();
    rect->SetSize(GetSize());
    rect->SetFillColor(black_color);
    rect->SetBorderColor(black_color);

    GetTexture().Draw(*rect);
    delete rect;
}

void ObjControlPanel::setObject(OptObject* obj)
{
    this->obj = obj;
    prop_cont->clearChildren();
    button_cont->clearChildren();

    if (obj == nullptr) name_text->SetText("");
    else name_text->SetText("Name");
    name_field->setObject(obj);

    if (obj != nullptr)
    {
        for (OptProperty prop: obj->getProperties()) {
            prop_cont->addChild(new OptPropWidget(GetUI(), {}, prop_cont->getChildSize(), obj, prop));
        }

        addMoveButton({0, 0, -obj_change}, "Forward");
        addMoveButton({0, 0, obj_change}, "Back");
        addMoveButton({-obj_change, 0, 0}, "Left");
        addMoveButton({obj_change, 0, 0}, "Right");
        addMoveButton({0, -obj_change, 0}, "Up");
        addMoveButton({0, obj_change, 0}, "Down");

        delete_button = new DeleteObjectButton(GetUI(), {0, GetSize().y * 0.8},
        {GetSize().x * 0.2, GetSize().y * 0.2}, nullptr, "Delete");
        delete_button->setObject(obj);
        button_cont->addChild(delete_button);
    }

    name_text->ForceRedraw();
    name_field->ForceRedraw();
    prop_cont->ForceRedraw();
    button_cont->ForceRedraw();
}

void ObjControlPanel::addMoveButton(Vector change, const std::string& text) {
    button_cont->addChild(new MoveObjectButton(GetUI(), {}, button_cont->getChildSize(), obj, change, text, this));
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

OptNameField::OptNameField(hui::UI *ui, dr4::Vec2f pos, dr4::Vec2f size, OptController* control)
    : InputField(ui, pos, size, black_color, ""), control(control) {
    //
}

void OptNameField::setObject(OptObject* obj) {
    this->obj = obj;
    if (obj == nullptr) SetText("");
    else SetText(obj->getName());
    ForceRedraw();
}

void OptNameField::action() {
    if (obj == nullptr) return;

    obj->setName(getText());
    for (Widget* w : control->obj_cont->children) {
        OptObjectButton* button = dynamic_cast<OptObjectButton*>(w);
        assert(button != nullptr);

        if (button->obj == obj) {
            button->SetText(getText());
            break;
        }
    }

}

OptPropWidget::OptPropWidget(hui::UI *ui, dr4::Vec2f pos, dr4::Vec2f size, OptObject* obj, OptProperty prop)
    : MyContainer(ui, pos, size)
{
    const int prop_field_padding = 5;

    name_field = new TextField(ui, {0, 0}, {GetSize().x * property_name_portion, GetSize().y});
    name_field->SetText(prop.getName());
    addChild(name_field);

    val_field = new OptPropField(this, ui, {GetSize().x * property_name_portion + prop_field_padding, 0},
        {GetSize().x * (1 - property_name_portion) - prop_field_padding, GetSize().y}, obj, prop);
    addChild(val_field);

    float incr_but_size = size.y / 2;
    addChild(new IncreaseValueButton(val_field, {size.x - incr_but_size, 0}, {incr_but_size, incr_but_size}, true));
    addChild(new IncreaseValueButton(val_field, {size.x - incr_but_size, incr_but_size}, {incr_but_size, incr_but_size}, false));
}

OptPropField::OptPropField(OptPropWidget* parent, hui::UI *ui, dr4::Vec2f pos, dr4::Vec2f size, OptObject* obj, OptProperty prop)
    : InputField(ui, pos, size, black_color, doubleToStr(prop.val)), obj(obj), prop(prop)
{
    setValidator([](std::string s) {
        try {
            double val = std::stod(s);
            return true;
        }
        catch(std::exception& e) {
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

IncreaseValueButton::IncreaseValueButton(OptPropField* field, dr4::Vec2f pos, dr4::Vec2f size, bool increase)
    : Button(field->GetUI(), pos, size, light_gray_color, increase ? "+" : "-"), field(field), increase(increase) {
    SetBorderColor(light_gray_color);
    SetAngleRadius(3);
}

void IncreaseValueButton::action() {
    double prev_val = std::stod(field->getText());
    field->SetText(doubleToStr(prev_val + (increase ? prop_increase_step : -prop_increase_step)));
    field->action();
}

hui::EventResult IncreaseValueButton::OnMouseDown(hui::MouseButtonEvent &evt) {
    Button::OnMouseDown(evt);
    return hui::EventResult::UNHANDLED;
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
    if (obj == nullptr) return;
    obj->movePos(change);

    panel->setDisplayedVal(OPT_POS_X, obj->pos.x);
    panel->setDisplayedVal(OPT_POS_Y, obj->pos.y);
    panel->setDisplayedVal(OPT_POS_Z, obj->pos.z);

    obj->scene->needsRerender();
}

DeleteObjectButton::DeleteObjectButton(hui::UI *ui, dr4::Vec2f pos, dr4::Vec2f size, OptObject* obj, std::string text)
    : Button(ui, pos, size, gray_color, text), obj(obj)
{
    //
}

void DeleteObjectButton::action()
{
    if (obj == nullptr) return;
    obj->scene->control->deleteObject(obj);
}

AddObjectButton::AddObjectButton(hui::UI *state, dr4::Vec2f pos, dr4::Vec2f size, OptObjectType type,
    std::string text, OptController* control)
    : Button(state, pos, size, gray_color, text), control(control)
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

OptController::OptController(hui::UI* ui, MyContainer* parent, dr4::Vec2f pos) : parent(parent)
{
    s = new OptScene(ui, nullptr, dr4::Vec2f(0, 0), dr4::Vec2f(scene_w, scene_w / ratio), this);
    scene_cvs_widget = new CanvasWidget(ui, pos, s);
    parent->addChild(scene_cvs_widget);

    panel = new ObjControlPanel(ui, pos + dr4::Vec2f(scene_w, obj_cont_h),
        {obj_list_w + obj_scroll_w, scene_h - obj_cont_h}, this);
    parent->addChild(panel);

    cam_cont = createCameraContainer(pos, {130, 100 * n_move_buttons});

    WContainer* add_but_cont = new WContainer(ui, pos + dr4::Vec2f(scene_w - tool_button_w, tools_h),
        {tool_button_w, 100 * 2}, 2, true);
    add_but_cont->addChild(new AddObjectButton(ui, {}, {}, OPT_OBJ_SHPERE, "Add sphere", this));
    add_but_cont->addChild(new AddObjectButton(ui, {}, {}, OPT_OBJ_SOURCE, "Add source", this));
    parent->addChild(add_but_cont);

    obj_cont = makeObjectContainer({0, 0}, {obj_list_w, obj_button_h});
    assert(obj_cont != nullptr);
    ScrollableWidget* obj_scrollable = new ScrollableWidget(obj_cont, pos + dr4::Vec2f(scene_w, 0), {obj_list_w, obj_cont_h});
    parent->addChild(obj_scrollable);

    obj_scroll = new WidgetScrollBar(ui, pos + dr4::Vec2f(scene_w + obj_list_w, 0),
        {obj_scroll_w, obj_cont_h}, obj_scrollable);
    parent->addChild(obj_scroll);
}

WContainer* OptController::createCameraContainer(dr4::Vec2f pos, dr4::Vec2f size) {
    hui::UI* ui = s->GetUI();
    WContainer* cam_cont = new WContainer(ui, pos, size, n_camera_buttons + n_save_buttons, 1);
    parent->addChild(cam_cont);

    cam_cont->addChild(new MoveCameraButton(ui, s, {0, 0, -cam_change_z}, gray_color, "Forward"));
    cam_cont->addChild(new MoveCameraButton(ui, s, {0, 0, cam_change_z}, gray_color, "Back"));
    cam_cont->addChild(new MoveCameraButton(ui, s, {-cam_change_x, 0, 0}, gray_color, "Left"));
    cam_cont->addChild(new MoveCameraButton(ui, s, {cam_change_x, 0, 0}, gray_color, "Right"));
    cam_cont->addChild(new MoveCameraButton(ui, s, {0, -cam_change_y, 0}, gray_color, "Up"));
    cam_cont->addChild(new MoveCameraButton(ui, s, {0, cam_change_y, 0}, gray_color, "Down"));
    cam_cont->addChild(new SaveRestoreButton(ui, {}, {}, this, true));
    cam_cont->addChild(new SaveRestoreButton(ui, {}, {}, this, false));

    return cam_cont;
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
    for (auto it = s->surfaces.begin(); it != s->surfaces.end(); ++it)
    {
        if (*it == obj)
        {
            s->surfaces.erase(it);
            break;
        }
    }

    for (auto it = s->sources.begin(); it != s->sources.end(); ++it)
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

OptObject* OptController::createObject(const std::string& type) {
    OptObject* obj = nullptr;

    switch (objTypeFromStr(type)) {
        case OPT_OBJ_SOURCE: {
            SphereSource* src = new SphereSource({}, {}, 1, "", s);
            s->sources.push_back(src);
            obj = src;
            break;
        }
        case OPT_OBJ_PLANE: {
            Surface* surface = new PlaneSurface(0, {}, "", s);
            s->surfaces.push_back(surface);
            obj = surface;
            break;
        }
        case OPT_OBJ_SHPERE: {
            Surface* surface = new SphereSurface({}, 1, {}, "", s);
            s->surfaces.push_back(surface);
            obj = surface;
            break;
        }
    }

    return obj;
}

OptObject* OptController::restoreObject(std::ifstream& file) {
    std::string type;
    file >> type;
    OptObject* obj = createObject(type);
    if (obj == nullptr) return nullptr;

    std::string obj_name;
    file.ignore();
    std::getline(file, obj_name);\
    obj->setName(obj_name);

    int n_properties = -1;
    file >> n_properties;
    for (int prop_num = 0; prop_num < n_properties; ++prop_num) {
        std::string prop_name;
        double prop_val = 0;
        file >> prop_name >> prop_val;

        obj->setProperty(propFromStr(prop_name), prop_val);
    }

    return obj;
}

int OptController::Restore(std::string path) {
    /*
    n_objects

    type
    name
    n_properties
    name1 val1
    name2 val2
    */

    s->selected.clear();
    s->sources.clear();
    s->surfaces.clear();
    panel->setObject(nullptr);
    obj_cont->clearChildren();

    std::ifstream file;
    file.open(path);
    if (!file.is_open()) return 1;

    int n_objects = -1;
    file >> n_objects;

    for (int obj_num = 0; obj_num < n_objects; obj_num++) {
        OptObject* obj = restoreObject(file);
        if (obj == nullptr) return 1;
        addObject(obj);
    }

    s->needsRerender();
    obj_cont->ForceRedraw();
    file.close();
    return 0;
}

int OptController::SaveObject(std::ofstream& file, OptObject* obj) {
    file << objTypeToStr(obj->type) << '\n' << obj->getName() << '\n';
    std::vector<OptProperty> properties = obj->getProperties();
    file << properties.size() << '\n';

    for (const OptProperty& prop: properties) {
        file << propToStr(prop.prop) << ' ' << prop.val << '\n';
    }
    file << '\n';

    return 0;
}

int OptController::Save(std::string path) {
    std::ofstream file;
    file.open(path);
    if (!file.is_open()) return 1;

    int n_objects = s->sources.size() + s->surfaces.size();
    file << n_objects << "\n\n";

    for (Surface* obj: s->surfaces) {
        int res = SaveObject(file, obj);
        if (res != 0) return res;
    }

    for (Source* obj: s->sources) {
        int res = SaveObject(file, obj);
        if (res != 0) return res;
    }

    file.close();
    return 0;
}

SaveRestoreButton::SaveRestoreButton(hui::UI *state, dr4::Vec2f pos, dr4::Vec2f size, OptController* control, bool save)
    : Button(state, pos, size, gray_color, save ? "Save" : "Restore"), save(save), control(control) {
    path = scene_data_path;
}

void SaveRestoreButton::action() {
    if (save) {
        assert(control->Save(path) == 0);
        return;
    }
    assert(control->Restore(path) == 0);
}
