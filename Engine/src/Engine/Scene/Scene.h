#pragma once

#include <Engine/Scene/EditorCamera.h>

#include <entt/entt.hpp>

#include "Engine/Core/TimeStep.h"
#include "Engine/Core/Core.h"
#include "Engine/Core/UUID.h"

class b2World;

namespace Engine
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		static Ref<Scene> Copy(Ref<Scene> scene);

		Entity CreateEntity(const std::string& tag);
		Entity CreateEntityWithUUID(UUID uuid, const std::string& tag);

		Entity CopyEntity(const Entity& entity);

		void DestroyEntity(Entity& entity);

		void Clean();

		void OnUpdateEditor(TimeStep ts, EditorCamera& camera);
		void OnUpdateRuntime(TimeStep ts);

		void OnViewResize(uint32_t width, uint32_t height);

		void OnRuntimeBegin();
		void OnRuntimeStop();

		Entity GetPrimaryCameraEntity();

		template<typename... Components>
		auto GetAllEntitiesWith()
		{
			return m_registry.view<Components...>();
		}

	private:
		entt::registry m_registry;

		b2World* m_physics_world{ nullptr };

		uint32_t m_viewport_width, m_viewport_height;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};
}
