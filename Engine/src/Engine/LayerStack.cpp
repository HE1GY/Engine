#include "pch.h"

#include "LayerStack.h"

namespace Engine
{
	LayerStack::LayerStack()
			:m_position{ 0 }
	{
	}
	LayerStack::~LayerStack()
	{
		for (auto& layer : m_layers)
		{
			delete layer;
		}
	}

	void LayerStack::PushLayer(Engine::Layer* layer)
	{
		m_layers.emplace(m_layers.begin() + m_position++, layer);
		layer->OnAttach();
	}
	void LayerStack::PushOverlay(Engine::Layer* layer)
	{
		m_layers.push_back(layer);
		layer->OnAttach();
	}
	void LayerStack::EraseLayer(Engine::Layer* layer)
	{
		auto position_iterator = std::find(m_layers.begin(), m_layers.end(), layer);
		if (position_iterator != m_layers.end())
		{
			m_layers.erase(position_iterator);
		}
		layer->OnDetach();
	}

} // namespace Engine
