#include "pch.h"

#include <imgui.h>
#include "glm/gtc/type_ptr.hpp"
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

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_selection_context = {};
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_selection_context)
		{ DrawEntityProperties(m_selection_context); }
		ImGui::End();
	}
	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tc = entity.GetComponent<TagComponent>();
		ImGuiTreeNodeFlags flag =
				((m_selection_context == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool open = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flag, tc.tag.c_str());

		if (ImGui::IsItemClicked())
		{
			m_selection_context = entity;

			//TODO event
		}

		if (open)
		{
			ImGui::TreePop();
		}

	}
	void SceneHierarchyPanel::DrawEntityProperties(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>();

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)(typeid(TransformComponent)).hash_code(), ImGuiTreeNodeFlags_DefaultOpen,
					"Transform"))
			{
				auto& transform_cmp = entity.GetComponent<TransformComponent>();
				ImGui::DragFloat3("Position", glm::value_ptr(transform_cmp.transform[3]), 0.1f);
				ImGui::TreePop();
			}

		}

		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)(typeid(CameraComponent)).hash_code(), ImGuiTreeNodeFlags_DefaultOpen,
					"Camera"))
			{
				auto& cam_cmp = entity.GetComponent<CameraComponent>();

				ImGui::Checkbox("Primary", &cam_cmp.primary);

				const char* projection_type_string[] = { "Perspective", "Orthographic" };
				const char* current_projection_type_string = projection_type_string[(int)cam_cmp.camera.get_projection_type()];
				if (ImGui::BeginCombo("Projection", current_projection_type_string))
				{
					for (int i = 0; i < 2; ++i)
					{
						bool is_selected = current_projection_type_string == projection_type_string[i];
						if (ImGui::Selectable(projection_type_string[i], is_selected))
						{
							current_projection_type_string = projection_type_string[i];
							cam_cmp.camera.set_projection_type((SceneCamera::ProjectionType)i);
						}

						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (cam_cmp.camera.get_projection_type() == SceneCamera::ProjectionType::Perspective)
				{
					float persp_FOV = glm::degrees(cam_cmp.camera.get_perspective_FOV());
					float persp_near_clip = cam_cmp.camera.get_perspective_near_clip();
					float persp_far_clip = cam_cmp.camera.get_perspective_far_clip();

					ImGui::DragFloat("FOV", &persp_FOV);
					ImGui::DragFloat("Near clip", &persp_near_clip);
					ImGui::DragFloat("Far clip", &persp_far_clip);

					cam_cmp.camera.set_perspective_FOV(glm::radians(persp_FOV));
					cam_cmp.camera.set_perspective_near_clip(persp_near_clip);
					cam_cmp.camera.set_perspective_far_clip(persp_far_clip);
				}

				if (cam_cmp.camera.get_projection_type() == SceneCamera::ProjectionType::Orthographic)
				{
					float ortho_size = cam_cmp.camera.get_orthographic_size();
					float ortho_near_clip = cam_cmp.camera.get_orthographic_near_clip();
					float ortho_far_clip = cam_cmp.camera.get_orthographic_far_clip();

					ImGui::DragFloat("Size", &ortho_size);
					ImGui::DragFloat("Near clip", &ortho_near_clip);
					ImGui::DragFloat("Far clip", &ortho_far_clip);

					cam_cmp.camera.set_orthographic_size(ortho_size);
					cam_cmp.camera.set_orthographic_near_clip(ortho_near_clip);
					cam_cmp.camera.set_orthographic_far_clip(ortho_far_clip);

					ImGui::Checkbox("Fixed aspect ratio", &cam_cmp.fix_aspectratio);

				}

				ImGui::TreePop();
			}

		}

	}
}