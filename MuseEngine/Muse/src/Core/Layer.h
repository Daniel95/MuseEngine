#pragma once

#include "Timestep.h"

#include <string>

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

		const std::string& GetName() const { return debugName; }
	protected:
		std::string debugName;

	};
}
