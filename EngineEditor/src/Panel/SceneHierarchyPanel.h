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

	private:
		void DrawEntityNode(Entity entity);
		void DrawEntityProperties(Entity entity);

		Ref<Scene> m_context;
		Entity m_selection_context;
	};
}
