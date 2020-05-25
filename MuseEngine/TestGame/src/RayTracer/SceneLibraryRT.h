#pragma once

#include <memory>

namespace Muse {
    class Scene;
}

class SceneLibraryRT
{
public:
    static void MakeRTBenchmarkScene(std::shared_ptr<Muse::Scene> a_Scene);
    static void MakePTBenchmarkScene(std::shared_ptr<Muse::Scene> a_Scene);
    static void MakePtSkyBox(std::shared_ptr<Muse::Scene> a_Scene, float a_Width, float a_Height, float a_Length);

};
