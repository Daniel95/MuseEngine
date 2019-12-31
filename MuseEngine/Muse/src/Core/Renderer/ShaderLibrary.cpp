#include "MusePCH.h"
#include "ShaderLibrary.h"
#include "Shader.h"
#include "Core/Utilities/Defines.h"

namespace Muse
{
    void ShaderLibrary::Add(const std::shared_ptr<Shader>& a_Shader)
    {
        auto& name = a_Shader->GetName();
        Add(name, a_Shader);
    }

    void ShaderLibrary::Add(const std::string& a_Name, const std::shared_ptr<Shader>& a_Shader)
    {
        ASSERT_ENGINE(m_Shaders.find(a_Name) == m_Shaders.end(), "Shader already exists!");
        m_Shaders[a_Name] = a_Shader;
    }

    std::shared_ptr<Shader> ShaderLibrary::Load(const std::string& a_FilePath)
    {
    #ifdef MUSE_DEBUG 
        if (!std::experimental::filesystem::exists(a_FilePath))
        {
            LOG_ENGINE_ERROR("Shader does not exists! {0}", a_FilePath);
            _ASSERT(false, "Shader does not exists!");
        }
    #endif

        auto shader = Shader::Create(a_FilePath);
        Add(shader);
        return shader;
    }

    std::shared_ptr<Shader> ShaderLibrary::Load(const std::string& a_Name, const std::string& a_FilePath)
    {
    #ifdef MUSE_DEBUG 
        if (!std::experimental::filesystem::exists(a_FilePath))
        {
            LOG_ENGINE_ERROR("Shader does not exists! {0}", a_FilePath);
            _ASSERT(false, "Shader does not exists!");
        }
    #endif

        auto shader = Shader::Create(a_FilePath);
        Add(a_Name, shader);
        return shader;
    }
}
