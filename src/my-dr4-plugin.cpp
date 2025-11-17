#include "my-dr4-plugin.h"
#include "mywindow.h"

const int window_w = 1920, window_h = 1000;

namespace cum {

extern "C" cum::DR4BackendPlugin* CreateDR4Backend(void)
{
    return new cum::AbraCat_DR4Backend();
}

AbraCat_DR4Backend::AbraCat_DR4Backend()
{
    name = description = "AbraCat dr4 plugin";
    // descr_view = description;
}

std::string_view AbraCat_DR4Backend::GetName() const
{
    return name;
}

dr4::Window * AbraCat_DR4Backend::CreateWindow()
{
    return new dr4::MyWindow(dr4::Vec2f(window_w, window_h), "Optical Constructor");
}

std::string_view AbraCat_DR4Backend::GetIdentifier() const { return name; }
std::string_view &AbraCat_DR4Backend::GetDescription() const {std::string_view str(description); return str; }
std::vector<std::string_view> AbraCat_DR4Backend::GetDependencies() const { return std::vector<std::string_view>(); }
std::vector<std::string_view> AbraCat_DR4Backend::GetConflicts() const { return std::vector<std::string_view>(); }
void AbraCat_DR4Backend::AfterLoad() {}

} // namespace cum
