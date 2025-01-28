#include "UISystem.hpp"

namespace Snowglobe::Render
{
    UIPanel::~UIPanel()
    {
        _system->Close(this);
    }
}


