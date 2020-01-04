#pragma once
#include "Core/Renderer/Shader.h"
#include "Core/Renderer/Texture.h"
#include "Core/Scene/Scene.h"
#include "Core/Scene/SceneManager.h"

namespace Muse
{
    class ResourceTest
    {
    public:
        template<typename T, typename ... Args>
        static std::shared_ptr<T> Create(const std::string& a_Name, Args ... a_Args)
        {
            return Create<T>(a_Args ...);
        }

        template<typename T>
        static std::shared_ptr<T> Load(const std::string& a_FilePath)
        {
            return Load<T>(a_FilePath);
        }

        template<typename T>
        static std::shared_ptr<T> CreateResource() { }
        template<typename T>
        static std::shared_ptr<T> CreateResource(const std::string& a_VertexSource, const std::string& a_FragmentSource) { return Shader::Create(a_VertexSource, a_FragmentSource); }
        template<typename T>
        static std::shared_ptr<T> CreateResource(uint32_t a_Width, uint32_t a_Height) { return Texture::Create(a_Width, a_Height); }
        template<typename T>
        static std::shared_ptr<T> CreateResource(const std::string& a_SceneName) { return SceneManager::NewScene(); }

        template<typename T>
        static std::shared_ptr<T> LoadResource(const std::string& a_FilePath) { return nullptr; }
        template<>
        static std::shared_ptr<Shader> LoadResource<Shader>(const std::string& a_FilePath) { return Shader::Load(a_FilePath); }
        template<>
        static std::shared_ptr<Texture> LoadResource<Texture>(const std::string& a_FilePath) { return Texture::Load(a_FilePath); }
        template<>
        static std::shared_ptr<Scene> LoadResource<Scene>(const std::string& a_FilePath) { return SceneManager::NewScene(); }


        template <typename T, typename ... Args>
        T fun(Args ... a_Args) {  }

        // acts like partial specialization <T, int> AFAIK 
        // (based on Modern C++ Design by Alexandrescu)
        template <typename T>
        T fun(int pObj, int test) { }




        /*
        template< class T, class U >
        struct F
        {
            static void impl() { say("1. primary template"); }
        };

        template<>
        struct F<int, char>
        {
            static void impl() { say("2. <int, char> explicit specialization"); }
        };

        template< class T >
        struct F< char, T >
        {
            static void impl() { say("3. <char, T> partial specialization"); }
        };

        template< class T >
        struct F< T, int >
        {
            static void impl() { say("4. <T, int> partial specialization"); }
        };
        */

    };
}