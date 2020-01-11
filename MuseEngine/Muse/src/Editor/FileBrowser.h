#pragma once

namespace Muse
{
    class FileBrowser
    {
    public:
        static void Render();

    private:
        static void InspectResourceFiles(const std::string& a_Path);

    };
}

