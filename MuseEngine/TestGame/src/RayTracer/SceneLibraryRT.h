#pragma once

#include <memory>

namespace Muse {
    class Scene;
}

class SceneLibraryRT
{
public:
    static void MakeTestScene(std::shared_ptr<Muse::Scene> a_Scene);

};
