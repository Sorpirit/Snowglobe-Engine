#include "UISystem.hpp"

#include "RenderSystem.hpp"

namespace Snowglobe::Render
{
    UIPanel::~UIPanel()
    {
        _system->Close(this);
    }
} // namespace Snowglobe::Render


