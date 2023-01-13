#pragma once

#include "Engine/Core/TimeStep.h"
#include "Engine/Event/Event.h"

namespace Engine
{
	class Layer
	{
	public:
		Layer(std::string name = "Layer")
				:m_name{ name }
		{
		}
		virtual ~Layer() = default;
		virtual void OnAttach()
		{
		}
		virtual void OnDetach()
		{
		}
		virtual void OnUpdate(TimeStep ts)
		{
		}
		virtual void OnEvent(Event& event)
		{
		}

	private:
		std::string m_name;
	};
}

