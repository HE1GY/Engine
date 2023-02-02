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
		void DestroyEntity(Entity& entity);

		void OnUpdate(TimeStep ts);
		void OnViewResize(uint32_t width, uint32_t height);

		Entity GetPrimaryCameraEntity();

	private:
		entt::registry m_registry;

		uint32_t m_viewport_width, m_viewport_height;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};
}
