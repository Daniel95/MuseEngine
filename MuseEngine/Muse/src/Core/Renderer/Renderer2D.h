#pragma once

#include <memory>
#include <array>
#include "glm/glm.hpp"

namespace Muse
{
    class CameraComponent;
    class Texture;
    class VertexArray;
    class VertexBuffer;
    class Shader;

    class Renderer2D
    {
    public:
        struct Statistics
        {
            uint32_t DrawCalls = 0;
            uint32_t QuadCount = 0;

            uint32_t GetTotalVertex() { return QuadCount * 4; };
            uint32_t GetTotalIndex() { return QuadCount * 6; };
        };

        struct QuadVertex
        {
            glm::vec3 Position;
            glm::vec4 Color;
            glm::vec2 TexCoord;
            float TexIndex;
            float TilingFactor;
        };

        struct Data
        {
            static const uint32_t MaxQuads = 5000;
            static const uint32_t MaxVertices = MaxQuads * 4;
            static const uint32_t MaxIndices = MaxQuads * 6;
            static const uint32_t MaxTextureSlots = 32; // TODO: RenderCaps

            std::shared_ptr<VertexArray> QuadVertexArray;
            std::shared_ptr<VertexBuffer> QuadVertexBuffer;
            std::shared_ptr<Shader> ColoredTextureShader;
            std::shared_ptr<Texture> WhiteTexture;

            uint32_t QuadIndexCount = 0;
            QuadVertex* QuadVertexBufferPtr = nullptr;
            QuadVertex* QuadVertexBufferBase = nullptr;

            std::array<std::shared_ptr<Texture>, MaxTextureSlots> TextureSlots;
            uint16_t TextureSlotIndex = 1; // 0 = white texture

            glm::vec4 QuadVertexPositions[4];
            glm::vec2 TextureCoordinates[4];

            Statistics Stats;
        };

        static void Init();
        static void ShutDown();

        static void BeginScene(const CameraComponent& a_OrthographicCamera);
        static void EndScene();
        static void Flush();

        static void DrawQuad(const glm::vec3& a_Position, const glm::vec2& a_Size, float a_Rotation, const glm::vec4& a_Color = glm::vec4(1));
        static void DrawQuad(const glm::vec3& a_Position, const glm::vec2& a_Size, float a_Rotation, const std::shared_ptr<Texture>& a_Texture, float a_TilingFactor = 1, const glm::vec4& a_TintColor = glm::vec4(1));
        static void DrawQuad(const glm::mat4& a_Transform, const glm::vec4& a_Color = glm::vec4(1));
        static void DrawQuad(const glm::mat4& a_Transform, const std::shared_ptr<Texture>& a_Texture, float a_TilingFactor = 1, const glm::vec4& a_TintColor = glm::vec4(1));

        static Statistics GetStatistics() { return s_Data.Stats; }
        static void ResetStatistics() { memset(&s_Data.Stats, 0, sizeof(Statistics)); }

    private:
        static Data s_Data;

        static void DrawQuad(const glm::mat4& a_Transform, const glm::vec4& a_TintColor, int a_TextureIndex, float a_TilingFactor);
        static void Reset();
    };
}
