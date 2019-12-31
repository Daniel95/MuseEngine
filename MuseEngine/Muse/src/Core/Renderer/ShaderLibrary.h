#pragma once

#include <memory>
#include <unordered_map>
#include <string>

namespace Muse
{
    class Shader;

    class ShaderLibrary
    {
    public:
        void Add(const std::shared_ptr<Shader>& a_Shader);
        void Add(const std::string& a_Name, const std::shared_ptr<Shader>& a_Shader);
        std::shared_ptr<Shader> Load(const std::string& a_FilePath);
        std::shared_ptr<Shader> Load(const std::string& a_Name, const std::string& a_FilePath);

        std::shared_ptr<Shader> Get(const std::string& a_Name) { return m_Shaders.at(a_Name); }
        bool Exists(const std::string& a_Name) { return m_Shaders.find(a_Name) != m_Shaders.end(); }

    private:
        std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
    };
}
