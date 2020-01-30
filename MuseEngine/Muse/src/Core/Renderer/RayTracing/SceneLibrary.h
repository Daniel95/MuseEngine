#pragma once

#include <vector>
#include <memory>
#include "SceneObject.h"
#include "Scene.h"


namespace Muse
{

    Scene& GetTestingScene();
    Scene& GetWhittedImageScene();
    Scene& GetOBBTestScene();
    Scene& ReferenceImage2();
    Scene& GetPerformanceTestScene1(bool bvh = true, int boxesAmount = 10);
    Scene& GetPerformanceTestScene2(bool bvh = true, int boxesAmount = 10);
}