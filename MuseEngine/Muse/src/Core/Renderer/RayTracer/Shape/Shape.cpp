#include "MusePCH.h"

#include "Core/Renderer/RayTracer/Shape/Shape.h"
#include "Core/Gameplay/GameObject.h"

namespace Muse
{
    void Shape::OnInit()
    {
        Component::OnInit();

        m_TransformComponent = GetGameObject()->GetTransform();
    }
}

