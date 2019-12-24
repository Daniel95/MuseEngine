#pragma once

#include "Core/Timestep.h"

#include <string>

namespace Muse
{
	class Layer
	{
	public:
		Layer(const std::string& a_Name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep a_Timestep) {}
		virtual void OnImGuiRender() {}

		const std::string& GetName() const { return debugName; }
	protected:
		std::string debugName;

	};
}
