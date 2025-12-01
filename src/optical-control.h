#ifndef OPTICAL_CONTROL_H
#define OPTICAL_CONTROL_H

#include "optical-scene.h"
#include "button.h"
#include "wcontainer.h"


class ObjControlPanel;
class OptPropWidget;

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
    virtual void action() override;

private:
    OptObject* obj;
};



class ObjControlPanel : public MyContainer
{
public:
    ObjControlPanel(hui::UI *state, dr4::Vec2f pos, dr4::Vec2f size);
    void setObject(OptObject* obj);
    void setDisplayedVal(OptPropEnum prop, double val);

private:
    OptObject *obj;
    WContainer *prop_cont, *button_cont;
};

class OptController
{
public:
    OptController(hui::UI* ui, MyContainer* parent);
    WContainer* makeObjectContainer(dr4::Vec2f pos, dr4::Vec2f size);

    void addObject(OptObject* obj);
    std::vector<Surface*>::iterator addSphere(Vector pos, Vector color, double r, Material m = plastic);
    std::vector<Source*>::iterator addSource(Vector pos, Vector color, double r);
    
    void select(OptObject* obj);
    void deselect(OptObject* obj);
    void selected_changed();
    void deleteObject(OptObject* obj);

    OptScene* getScene() const { return s; }
    pp::MyCanvas* getCanvas() const { return cvs_widget->getCanvas(); }

    hui::Widget* parent;

    OptScene* s;
    CanvasWidget* cvs_widget;
    MyContainer *cam_cont;
    // WList *obj_cont;
    WContainer* obj_cont;
    // ListScrollBar *obj_scroll;
    ObjControlPanel* panel;
};

// } // namespace hui

#endif // OPTICAL_CONTROL_H