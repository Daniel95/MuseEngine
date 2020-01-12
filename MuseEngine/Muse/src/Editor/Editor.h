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

        static std::string GetOpenPath(const std::string& a_Filter);
        static std::wstring GetSavePath(const std::wstring& a_Filter, const std::wstring& a_Extension);

    private:
        static bool s_EditorMode;
    };
}
