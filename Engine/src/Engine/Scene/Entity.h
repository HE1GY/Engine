#pragma once

#include "entt/entt.hpp"
#include "Engine/Core/Log.h"
#include "Engine/Core/Core.h"
#include "Scene.h"

namespace Engine
{
	class Entity
	{

	public:
		Entity() = default;
		Entity(entt::entity entity_handler, Scene* scene)
				:
				m_entity_handler{ entity_handler }, m_scene{ scene }
		{
		};

		template<typename T>
		T& GetComponent()
		{
			CORE_ASSERT(HasComponent<T>(), "Entity doesn't have component");
			return m_scene->m_registry.get<T>(m_entity_handler);
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&& ... args)
		{
			CORE_ASSERT(!HasComponent<T>(), "Entity already has component");
			return m_scene->m_registry.emplace<T>(m_entity_handler, std::forward<Args>(args)...);
		}

		template<typename T>
		void RemoveComponent()
		{
			CORE_ASSERT(HasComponent<T>(), "Entity doesn't have component");
			m_scene->m_registry.erase<T>(m_entity_handler);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_scene->m_registry.any_of<T>(m_entity_handler);
		}

		operator bool()
		{
			return m_entity_handler != entt::null;
		}

	private:

		entt::entity m_entity_handler{ entt::null };
		Scene* m_scene{ nullptr };
	};
}
