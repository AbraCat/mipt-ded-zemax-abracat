#include "my-dr4-plugin.h"
#include "mywindow.h"

const int window_w = 1920, window_h = 1000;

extern "C" dr4::DR4Backend* CreateDR4Backend(void)
{
    return new dr4::AbraCat_DR4Backend();
}

dr4::AbraCat_DR4Backend::AbraCat_DR4Backend()
{
    name = "AbraCat dr4 plugin";
}

const std::string& dr4::AbraCat_DR4Backend::Name() const
{
    return name;
}

dr4::Window * dr4::AbraCat_DR4Backend::CreateWindow()
{
    return new dr4::MyWindow(dr4::Vec2f(window_w, window_h), "Optical Constructor");
}
