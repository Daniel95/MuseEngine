#pragma once

namespace Muse
{
    class CameraComponent;

    class Renderer2D
    {
    public:
        static void Init();
        static void ShutDown();

        static void BeginScene(const CameraComponent& a_OrthographicCamera);
        static void EndScene();

        //Primitives:

    };
}
