#pragma once
#include <string>

// Utility extension for ImGui to accept strings besides just const char*. 
namespace ImGui
{
    bool InputText(std::string a_Label, std::string& a_Buffer, int a_Flags = 1 << 18); // int a_Flags == ImGuiInputTextFlags a_Flags
    bool InputText(const char* a_Label, std::string& a_Buffer, int a_Flags = 1 << 18); // 1 << 18 == ImGuiInputTextFlags_CallbackResize
}
