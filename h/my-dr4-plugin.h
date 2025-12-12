#ifndef I_ABRACAT_DR4_PLUGIN
#define I_ABRACAT_DR4_PLUGIN

#include "cum/ifc/dr4.hpp"

namespace cum
{

class AbraCat_DR4Backend : public cum::DR4BackendPlugin
{
public:
    AbraCat_DR4Backend();
    virtual dr4::Window *CreateWindow() override;

    virtual std::string_view GetIdentifier() const override;
    virtual std::string_view GetName() const override;
    virtual std::string_view GetDescription() const override;
    virtual std::vector<std::string_view> GetDependencies() const override;
    virtual std::vector<std::string_view> GetConflicts() const override;
    virtual void AfterLoad() override;

private:
    std::string name, description;
};

} // namespace cum

extern "C" cum::Plugin *CreatePlugin();

#endif // I_ABRACAT_DR4_PLUGIN
