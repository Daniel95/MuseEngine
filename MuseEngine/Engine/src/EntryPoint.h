#pragma once

#include "Core/Application.h"
#include "Core/Utilities/Log.h"

#include "Examples/RTTRExample.h"

#include <cstdint>

extern Muse::Application* Muse::CreateApplication();

int main()
{
    Muse::Log::Init();

    LOG_ENGINE_WARN("Initialized Log!");

    Muse::Application* application = Muse::CreateApplication();
    application->Start();

    delete application;

    return 1;
}