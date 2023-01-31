#include "pch.h"

#include <imgui.h>
#include "glm/gtc/type_ptr.hpp"
#include <Engine/Scene/Components.h>
#include <imgui_internal.h>
#include "SceneHierarchyPanel.h"

namespace Engine
{
	template<typename T>
	void SceneHierarchyPanel::DrawComponent(const char* label, Entity& entity, std::function<void(T&)> drawFn)
	{
		if (entity.HasComponent<T>())
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4, });
			bool open = ImGui::TreeNodeEx((void*)(typeid(T)).hash_code(), ImGuiTreeNodeFlags_DefaultOpen,
					label);

			bool destroy_component = false;
			if (typeid(T) != typeid(TransformComponent) && typeid(T) != typeid(TagComponent))
			{
				ImGui::SameLine(ImGui::GetWindowWidth() - 25.0f);

				if (ImGui::Button("+", ImVec2(20, 20)))
				{
					ImGui::OpenPopup("ComponentSettings");
				}

				if (ImGui::BeginPopup("ComponentSettings"))
				{
					if (ImGui::MenuItem("Remove component"))
					{
						destroy_component = true;
					}

					ImGui::EndPopup();
				}
			}
			ImGui::PopStyleVar();

			if (open)
			{
				auto& tag = entity.GetComponent<T>();
				drawFn(tag);
				ImGui::TreePop();
			}

			if (destroy_component)
			{
				entity.RemoveComponent<T>();
			}
		}
	}

	static void
	DrawVec3Control(const std::string& label, glm::vec3& values, float reset_value = 0.0f, float column_width = 100.0f)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, column_width);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { line_height + 3.0f, line_height };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });

		if (ImGui::Button("X", buttonSize))
		{
			values.x = reset_value;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		if (ImGui::Button("Y", buttonSize))
		{
			values.y = reset_value;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		if (ImGui::Button("Z", buttonSize))
		{
			values.z = reset_value;
		}
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

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


		//click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("Create empty entity"))
			{
				m_context->CreateEntity("Empty entity");
			}
			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_selection_context)
		{
			DrawEntityProperties(m_selection_context);

			if (ImGui::Button("Add Component"))
			{
				ImGui::OpenPopup("AddComponent");
			}

			if (ImGui::BeginPopup("AddComponent"))
			{
				if (ImGui::MenuItem("Camera"))
				{
					m_selection_context.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("SpriteRenderer"))
				{
					m_selection_context.AddComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("NativeScript"))
				{
					m_selection_context.AddComponent<NativeScriptComponent>();
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

		}
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

		bool destroy = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Destroy entity"))
			{
				destroy = true;
			}
			ImGui::EndPopup();
		}

		if (open)
		{
			ImGui::TreePop();
		}

		if (destroy)
		{
			if (m_selection_context == entity)
			{
				m_selection_context = {};
			}
			m_context->DestroyEntity(entity);
		}

	}
	void SceneHierarchyPanel::DrawEntityProperties(Entity entity)
	{

		DrawComponent<TagComponent>("Tag", entity, [](TagComponent& tag_component)
		{
		  char buffer[256];
		  memset(buffer, 0, sizeof(buffer));
		  strcpy(buffer,/* sizeof(buffer), */tag_component.tag.c_str());//on linux
		  if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
		  {
			  tag_component = std::string(buffer);
		  }
		});

		DrawComponent<TransformComponent>("Transform", entity, [](TransformComponent& transform_component)
				{

				  DrawVec3Control("Translation", transform_component.translation);

				  glm::vec3 rot = glm::degrees(transform_component.rotation);
				  DrawVec3Control("Rotation", rot);
				  transform_component.rotation = glm::radians(rot);

				  DrawVec3Control("Scale", transform_component.scale, 1.0f);
				}
		);

		DrawComponent<CameraComponent>("Camera", entity, [](CameraComponent& cam_cmp)
		{
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

			  ImGui::Checkbox("Fixed aspect ratio", &cam_cmp.fix_aspect_ratio);

		  }
		});

		DrawComponent<SpriteRendererComponent>("SpriteRenderer", entity, [](SpriteRendererComponent& sprite_renderer)
		{
		  ImGui::ColorEdit4("Color", glm::value_ptr(sprite_renderer.color));
		});
	}
}