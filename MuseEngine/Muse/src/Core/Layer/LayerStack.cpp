#include "MusePCH.h"
#include "LayerStack.h"
#include "Layer.h"
#include "Core/Instrumentor.h"

namespace Muse
{
	LayerStack::~LayerStack()
	{
		MUSE_PROFILE_FUNCTION();

		for (Layer* layer : m_Layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::PushLayer(Muse::Layer* a_Layer)
	{
		MUSE_PROFILE_FUNCTION();

		m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, a_Layer);
		m_LayerInsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* a_Overlay)
	{
		MUSE_PROFILE_FUNCTION();

		m_Layers.emplace_back(a_Overlay);
	}

	void LayerStack::PopLayer(Layer* a_Layer)
    {
		MUSE_PROFILE_FUNCTION();

		auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, a_Layer);
		if (it != m_Layers.begin() + m_LayerInsertIndex)
		{
			a_Layer->OnDetach();
			m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* a_Overlay)
	{
		MUSE_PROFILE_FUNCTION();

		auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), a_Overlay);
		if (it != m_Layers.end())
		{
			a_Overlay->OnDetach();
			m_Layers.erase(it);
		}
	}
}
