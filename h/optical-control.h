#ifndef OPTICAL_CONTROL_H
#define OPTICAL_CONTROL_H

#include "optical-scene.h"
#include "button.h"
#include "wcontainer.h"
#include "scroll.h"


class ObjControlPanel;
class OptPropWidget;

class OptNameField : public InputField
{
public:
    OptNameField(hui::UI *ui, dr4::Vec2f pos, dr4::Vec2f size, OptController* control);
    void setObject(OptObject* obj);
    virtual void action() override;

// private:
    OptObject* obj;
    OptController* control;
};

class OptPropField : public InputField
{
public:
    OptPropField(OptPropWidget* parent, hui::UI *ui, dr4::Vec2f pos, dr4::Vec2f size, OptObject* obj, OptProperty prop);
    virtual void action() override;

// private:
    OptObject* obj;
    OptProperty prop;
};

class OptPropWidget : public MyContainer
{
public:
    OptPropWidget(hui::UI *ui, dr4::Vec2f pos, dr4::Vec2f size, OptObject* obj, OptProperty prop);

// private:
    TextField *name_field;
    OptPropField *val_field;
};

class OptObjectButton : public ToggleButton
{
public:
    OptObjectButton(hui::UI *ui, dr4::Vec2f pos, dr4::Vec2f size, OptObject* obj, ObjControlPanel* panel);
    virtual void action() override;
    virtual void deactivate() override;

// private:
    OptObject* obj;
};

class MoveCameraButton : public Button
{
public:
    MoveCameraButton(hui::UI* ui, OptScene* scene, Vector change, dr4::Color color, std::string text);
    virtual void action() override;

private:
    OptScene* scene;
    Vector change;
};

class MoveObjectButton : public Button
{
public:
    MoveObjectButton(hui::UI *state, dr4::Vec2f pos, dr4::Vec2f size, OptObject* obj, Vector change,
        std::string text, ObjControlPanel* panel);

    virtual void action() override;

private:
    OptObject* obj;
    Vector change;
    ObjControlPanel* panel;
};

class DeleteObjectButton : public Button
{
public:
    DeleteObjectButton(hui::UI *state, dr4::Vec2f pos, dr4::Vec2f size, OptObject* obj, std::string text);
    void setObject(OptObject* obj) { this->obj = obj; }
    virtual void action() override;

private:
    OptObject* obj;
};

class AddObjectButton : public Button
{
public:
    AddObjectButton(hui::UI *state, dr4::Vec2f pos, dr4::Vec2f size, OptObjectType type,
        std::string text, OptController* control);
    virtual void action() override;

private:
    OptController* control;
    OptObjectType type;
};



class ObjControlPanel : public MyContainer
{
public:
    ObjControlPanel(hui::UI *state, dr4::Vec2f pos, dr4::Vec2f size, OptController* control);
    void setObject(OptObject* obj);
    void setDisplayedVal(OptPropEnum prop, double val);
    virtual void Redraw() const override;

private:
    OptObject *obj;
    TextField* name_text;
    OptNameField* name_field;
    DeleteObjectButton* delete_button;
    WContainer *prop_cont;
    OptController* control;
    GridContainer *button_cont;
};

class OptController
{
public:
    OptController(hui::UI* ui, MyContainer* parent);
    WList* makeObjectContainer(dr4::Vec2f pos, dr4::Vec2f size);

    int Save(const char* path);
    int Restore(const char* path);

    void addObject(OptObject* obj);
    std::vector<Surface*>::iterator addSphere(Vector pos, Vector color, double r, Material m = plastic);
    std::vector<Source*>::iterator addSource(Vector pos, Vector color, double r);
    
    void select(OptObject* obj);
    void deselect(OptObject* obj);
    void selected_changed();
    void deleteObject(OptObject* obj);

    OptScene* getScene() const { return s; }
    pp::MyCanvas* getCanvas() const { return scene_cvs_widget->getCanvas(); }

// private:
    hui::Widget* parent;

    OptScene* s;
    CanvasWidget* scene_cvs_widget;
    MyContainer *cam_cont;
    WList *obj_cont;
    WidgetScrollBar *obj_scroll;
    ObjControlPanel* panel;
};

// } // namespace hui

#endif // OPTICAL_CONTROL_H