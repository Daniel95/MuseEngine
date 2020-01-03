#pragma once

#include <memory>

namespace Muse
{
    class Texture;
    class CameraComponent;

    class Renderer2D
    {
    public:
        static void Init();
        static void ShutDown();

        static void BeginScene(const CameraComponent& a_OrthographicCamera);
        static void EndScene();

        //Primitives:

        static void DrawQuad(const glm::vec2& a_Position, const glm::vec2& a_Size, std::shared_ptr<Texture> a_Texture);
        static void DrawQuad(const glm::vec3& a_Position, const glm::vec2& a_Size, std::shared_ptr<Texture> a_Texture);
        static void DrawQuad(const glm::mat4& a_Transform, std::shared_ptr<Texture> a_Texture);
        static void DrawQuad(const glm::vec2& a_Position, const glm::vec2& a_Size, const glm::vec4& a_Color);
        static void DrawQuad(const glm::vec3& a_Position, const glm::vec2& a_Size, const glm::vec4& a_Color);
        static void DrawQuad(const glm::mat4& a_Transform, const glm::vec4& a_Color);
    };
}
