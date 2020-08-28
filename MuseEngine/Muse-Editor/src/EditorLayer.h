#include "Muse.h"

namespace Muse
{
    class EditorLayer : public Layer
    {
    public:
        EditorLayer() = default;
        virtual ~EditorLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;

        virtual void OnUpdate(float deltaTime) override;
        virtual void OnImGuiRender() override;
    private:
        std::shared_ptr<FrameBuffer> m_Framebuffer;
        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
        float m_AspectRatio = 0;

        std::shared_ptr<Muse::Texture2D> m_SpriteSheet;
        std::shared_ptr<Muse::SubTexture2D> m_TreeTexture;

        uint32_t m_MapWidth, m_MapHeight;

        std::unordered_map<char, std::shared_ptr<Muse::SubTexture2D>> m_TextureMap;

        int m_CameraEntity;
        bool m_ViewportFocused = false, m_ViewportHovered = false;
    };
}