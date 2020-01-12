#pragma once

#include <string>

namespace Muse
{
    class Editor
    {
    public:

        static void StartDockSpace();
        static void EndDockSpace();

        static bool GetEditorMode() { return s_EditorMode; }
        static void SetEditorMode(bool a_EditorMode) { s_EditorMode = a_EditorMode; }

        static std::string OpenFile(const std::string& a_Filter);

    private:
        static bool s_EditorMode;
    };
}
