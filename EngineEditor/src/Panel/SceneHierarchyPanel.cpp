#include "pch.h"

#include <imgui.h>
#include <Engine/Scene/Components.h>
#include "SceneHierarchyPanel.h"

namespace Engine
{

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_context = context;
	}
	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		m_context->m_registry.each([&](auto entityID)
		{
		  Entity entity{ entityID, m_context.get() };
		  DrawEntityNode(entity);
		});

		ImGui::End();
	}
	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tc = entity.GetComponent<TagComponent>();
		ImGuiTreeNodeFlags flag =
				((m_selected == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool open = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flag, tc.tag.c_str());

		if (ImGui::IsItemClicked())
		{
			m_selected = entity;

			//TODO event
		}

		if (open)
		{
			ImGui::TreePop();
		}

	}
}