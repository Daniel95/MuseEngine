#pragma once

#include "System/EventSystem.h"
#include "Timestep.h"

namespace Muse
{
	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Muse::Event& event) {}

		inline const std::string& GetName() const { return debugName; }
	protected:
		std::string debugName;

	};
}
