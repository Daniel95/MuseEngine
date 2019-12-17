#pragma once

#include "Layer.h"

namespace Muse
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnUpdate(Timestep ts) override;
        virtual void OnEvent(EventOld& event) override;
    private:
        float m_Time;
    };
}
