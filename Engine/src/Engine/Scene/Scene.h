#pragma once

#include "entt/entt.hpp"
#include "Engine/Core/TimeStep.h"

namespace Engine
{
	class Entity;

	class Scene
	{
	public:
		Entity CreateEntity(const std::string& tag);

		void OnUpdate(TimeStep ts);
	private:
		entt::registry m_registry;

		friend class Entity;
	};
}
