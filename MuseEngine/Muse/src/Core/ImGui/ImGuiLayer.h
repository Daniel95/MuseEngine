#pragma once

#include "Core/Layer/Layer.h"

namespace Muse
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnImGuiRender() override;

        void Begin();
        void End();

        void SetBlockEvents(bool block) { m_BlockEvents = block; }
    private:
        bool m_BlockEvents = true;
        float m_Time;
    };
}
