#pragma once

#include <entt/entt.hpp>

#include "Engine/Core/Log.h"
#include "Engine/Core/Core.h"
#include "Scene.h"
#include "Components.h"

namespace Engine
{
	class Entity
	{

	public:
		Entity()
				:m_entity_handler{ entt::null }, m_scene{ nullptr }
		{
		};

		Entity(entt::entity entity_handler, Scene* scene)
				:m_entity_handler{ entity_handler }, m_scene{ scene }
		{
		};

		template<typename T>
		T& GetComponent() const
		{
			CORE_ASSERT(HasComponent<T>(), "Entity doesn't have component");
			return m_scene->m_registry.get<T>(m_entity_handler);
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&& ... args)
		{
			CORE_ASSERT(!HasComponent<T>(), "Entity already has component");
			T& component = m_scene->m_registry.emplace<T>(m_entity_handler, std::forward<Args>(args)...);
			return component;
		}

		template<typename T, typename... Args>
		T& AddOrReplaceComponent(Args&& ... args)
		{
			T& component = m_scene->m_registry.emplace_or_replace<T>(m_entity_handler, std::forward<Args>(args)...);
			return component;
		}

		template<typename T>
		void RemoveComponent()
		{
			CORE_ASSERT(HasComponent<T>(), "Entity doesn't have component");
			m_scene->m_registry.erase<T>(m_entity_handler);
		}

		template<typename T>
		bool HasComponent() const
		{
			return m_scene->m_registry.any_of<T>(m_entity_handler);
		}

		UUID GetUUID() const
		{
			return GetComponent<IDComponent>().uuid;
		}

		const std::string& GetName() const
		{
			return GetComponent<TagComponent>().tag;
		}

		operator bool()
		{
			return m_entity_handler != entt::null;
		}

		operator int32_t()
		{
			return (int32_t)m_entity_handler;
		}

		operator entt::entity()
		{
			return m_entity_handler;
		}

		bool operator==(const Entity& other) const
		{
			return other.m_entity_handler == m_entity_handler && m_scene == other.m_scene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

	private:

		entt::entity m_entity_handler{ entt::null };
		Scene* m_scene{ nullptr };
	};
}
