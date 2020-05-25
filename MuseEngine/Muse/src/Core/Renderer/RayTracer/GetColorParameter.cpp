#include "MusePCH.h"
#include "GetColorParameters.h"

namespace Muse
{
    const Material& GetColorParameters::GetMaterial()
    {
        return RenderComponent->GetMaterial();
    }

    std::shared_ptr<const Shape> GetColorParameters::GetShape()
    {
        return RenderComponent->GetShape();
    }
}