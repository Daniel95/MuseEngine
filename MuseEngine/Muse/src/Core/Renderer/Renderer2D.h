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

        struct QuadPropertiesTransform
        {
            QuadPropertiesTransform(const glm::mat4& a_Transform, const glm::vec4& a_Color)
                : Transform(a_Transform), Color(a_Color)
            {
            }
            QuadPropertiesTransform(const glm::mat4& a_Transform, std::shared_ptr<Texture> a_Texture, float a_TilingFactor = 1)
                : Transform(a_Transform), Texture(a_Texture), TilingFactor(a_TilingFactor)
            {
            }
            QuadPropertiesTransform(const glm::mat4& a_Transform, const glm::vec4& a_TintColor, std::shared_ptr<Texture> a_Texture, float a_TilingFactor = 1)
                : Transform(a_Transform), Color(a_TintColor), Texture(a_Texture), TilingFactor(a_TilingFactor)
            {
            }

            glm::mat4 Transform = glm::mat4(1);
            glm::vec4 Color = glm::vec4(1);
            std::shared_ptr<Texture> Texture = nullptr;
            float TilingFactor = 1;
        };

        struct QuadProperties
        {
            QuadProperties(const glm::vec3& a_Position, const glm::vec2& a_Size, float a_Rotation, const glm::vec4& a_Color)
                : Position(a_Position), Size(a_Size), Rotation(a_Rotation), Color(a_Color)
            {
            }
            QuadProperties(const glm::vec3& a_Position, const glm::vec2& a_Size, float a_Rotation, std::shared_ptr<Texture> a_Texture, float TilingFactor = 1)
                : Position(a_Position), Size(a_Size), Rotation(a_Rotation), Texture(a_Texture), TilingFactor(TilingFactor)
            {
            }
            QuadProperties(const glm::vec3& a_Position, const glm::vec2& a_Size, float a_Rotation, const glm::vec4& a_TintColor, std::shared_ptr<Texture> a_Texture, float TilingFactor = 1)
                : Position(a_Position), Size(a_Size), Rotation(a_Rotation), Color(a_TintColor), Texture(a_Texture), TilingFactor(TilingFactor)
            {
            }

            glm::vec3 Position = glm::vec3(0);
            glm::vec2 Size = glm::vec2(1);
            float Rotation = 0;
            glm::vec4 Color = glm::vec4(1);
            std::shared_ptr<Texture> Texture = nullptr;
            float TilingFactor = 1;
        };

        //static void DrawQuad(const QuadPropertiesTransform& a_QuadPropertiesTransform);
        //static void DrawQuad(const QuadProperties& a_QuadProperties);
        static void DrawQuad(const glm::vec3& a_Position, const glm::vec2& a_Size, const glm::vec4& a_Color);
        static void DrawQuad(const glm::vec3& a_Position, const glm::vec2& a_Size, const std::shared_ptr<Texture>& a_Texture, float a_TilingFactor = 1, const glm::vec4& a_TintColor = glm::vec4(1));
        static void DrawQuad(const glm::vec3& a_Position, const glm::vec2& a_Size, float a_Rotation, const std::shared_ptr<Texture>& a_Texture, float a_TilingFactor = 1, const glm::vec4& a_TintColor = glm::vec4(1));
    };
}
