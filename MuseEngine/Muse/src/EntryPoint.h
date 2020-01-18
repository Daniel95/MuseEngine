#pragma once

#include "Core/Application.h"
#include "Core/Utilities/Log.h"
#include "Examples/Cereal/CerealExample.h"

extern Muse::Application* Muse::CreateApplication();

int main(int argc, char** argv)
{
    MUSE_PROFILE_BEGIN_SESSION("Startup", "MuseProfile.json");

    TestSerializationCereal();

    Muse::Log::Init();

    LOG_ENGINE_INFO("Initialized Log!");

    Muse::Application* application = Muse::CreateApplication();
    application->Run();

    delete application;


    MUSE_PROFILE_END_SESSION();

    return 1;
}
