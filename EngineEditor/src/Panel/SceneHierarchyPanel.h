#pragma once

#include <Engine/Scene/Scene.h>
#include <Engine/Core/Core.h>
#include <Engine/Scene/Entity.h>

namespace Engine
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;

		void SetContext(const Ref<Scene>& context);
		void OnImGuiRender();

		Entity GetSelectedEntity();
		void SetSelectedEntity(Entity& entity);
	private:
		void DrawEntityNode(Entity entity);
		void DrawEntityProperties(Entity entity);

		Ref<Scene> m_current_scene;
		Entity m_selection_entity;
	};
}
