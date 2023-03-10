#include "pch.h"

#include <imgui.h>
#include "glm/gtc/type_ptr.hpp"
#include <Engine/Scene/Components.h>
#include <imgui_internal.h>
#include "SceneHierarchyPanel.h"

namespace Engine
{
	const ImGuiTreeNodeFlags tree_node_flags =
			ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_Framed
					| ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;

	template<typename T>
	void DrawComponent(const char* label, Entity& entity, std::function<void(T&)> drawFn)
	{
		if (entity.HasComponent<T>())
		{
			ImVec2 content_region = ImGui::GetContentRegionAvail();
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4, });
			float line_height = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)(typeid(T)).hash_code(), tree_node_flags,
					label);
			ImGui::PopStyleVar();

			ImGui::SameLine(content_region.x - line_height * 0.5f);

			if (ImGui::Button("+", ImVec2(line_height, line_height)))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool destroy_component = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove component"))
				{
					destroy_component = true;
				}

				ImGui::EndPopup();
			}

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
		ImGuiIO& io = ImGui::GetIO();
		auto bold_font = io.Fonts->Fonts[0];

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

		ImGui::PushFont(bold_font);
		if (ImGui::Button("X", buttonSize))
		{
			values.x = reset_value;
		}
		ImGui::PopFont();

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });

		ImGui::PushFont(bold_font);
		if (ImGui::Button("Y", buttonSize))
		{
			values.y = reset_value;
		}
		ImGui::PopFont();

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });

		ImGui::PushFont(bold_font);
		if (ImGui::Button("Z", buttonSize))
		{
			values.z = reset_value;
		}
		ImGui::PopFont();

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
		m_current_scene = context;
		m_selection_entity = {};
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		m_current_scene->m_registry.each([&](auto entityID)
		{
		  Entity entity{ entityID, m_current_scene.get() };
		  DrawEntityNode(entity);
		});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_selection_entity = {};
		}


		//click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("Create empty entity"))
			{
				m_current_scene->CreateEntity("Empty entity");
			}
			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_selection_entity)
		{
			DrawEntityProperties(m_selection_entity);

		}
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tc = entity.GetComponent<TagComponent>();
		ImGuiTreeNodeFlags flag =
				((m_selection_entity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow
						| ImGuiTreeNodeFlags_SpanAvailWidth;
		bool open = ImGui::TreeNodeEx((void*)(int64_t)(int32_t)entity, flag, tc.tag.c_str());

		if (ImGui::IsItemClicked())
		{
			m_selection_entity = entity;

		}

		bool destroy = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Destroy entity"))
			{
				destroy = true;
			}

			if (ImGui::MenuItem("Copy entity") && m_selection_entity)
			{
				m_current_scene->CopyEntity(m_selection_entity);
			}
			ImGui::EndPopup();
		}

		if (open)
		{
			ImGui::TreePop();
		}

		if (destroy)
		{
			if (m_selection_entity == entity)
			{
				m_selection_entity = {};
			}
			m_current_scene->DestroyEntity(entity);
		}

	}

	void SceneHierarchyPanel::DrawEntityProperties(Entity entity)
	{

		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>();
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy(buffer,/* sizeof(buffer), */tag.tag.c_str());//on linux
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(-1);
		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("AddComponent");
		}

		if (ImGui::BeginPopup("AddComponent"))
		{
			if (ImGui::MenuItem("Camera"))
			{
				m_selection_entity.AddComponent<CameraComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("SpriteRenderer"))
			{
				m_selection_entity.AddComponent<SpriteRendererComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("CircleRenderer"))
			{
				m_selection_entity.AddComponent<CircleRendererComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("LineRenderer"))
			{
				m_selection_entity.AddComponent<LineRendererComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("NativeScript"))
			{
				m_selection_entity.AddComponent<NativeScriptComponent>();
				ImGui::CloseCurrentPopup();

			}
			if (ImGui::MenuItem("Rigidbody2D"))
			{
				m_selection_entity.AddComponent<Rigidbody2DComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("BoxCollider2D"))
			{
				m_selection_entity.AddComponent<BoxCollider2DComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("CircleCollider2D"))
			{
				m_selection_entity.AddComponent<CircleCollider2DComponent>();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		if (entity.HasComponent<TransformComponent>())
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4, });
			bool open = ImGui::TreeNodeEx((void*)(typeid(TransformComponent)).hash_code(),
					tree_node_flags,
					"Transform");

			ImGui::PopStyleVar();

			if (open)
			{
				auto& tc = entity.GetComponent<TransformComponent>();
				DrawVec3Control("Translation", tc.translation);

				glm::vec3 rot = glm::degrees(tc.rotation);
				DrawVec3Control("Rotation", rot);
				tc.rotation = glm::radians(rot);

				DrawVec3Control("Scale", tc.scale, 1.0f);

				ImGui::TreePop();
			}
		}

		DrawComponent<CameraComponent>("Camera", entity, [](CameraComponent& cam_cmp)
		{
		  ImGui::Checkbox("Primary", &cam_cmp.primary);

		  const char* projection_type_string[] = { "Perspective", "Orthographic" };
		  const char* current_projection_type_string = projection_type_string[(int)cam_cmp.camera.GetProjectionType()];
		  ImGui::Text("Projection");
		  ImGui::SameLine();
		  if (ImGui::BeginCombo("##Projection", current_projection_type_string))
		  {
			  for (int i = 0; i < 2; ++i)
			  {
				  bool is_selected = current_projection_type_string == projection_type_string[i];
				  if (ImGui::Selectable(projection_type_string[i], is_selected))
				  {
					  current_projection_type_string = projection_type_string[i];
					  cam_cmp.camera.SetProjectionType((ProjectionType)i);
				  }

				  if (is_selected)
					  ImGui::SetItemDefaultFocus();
			  }

			  ImGui::EndCombo();
		  }

		  if (cam_cmp.camera.GetProjectionType() == ProjectionType::Perspective)
		  {
			  float persp_FOV = glm::degrees(cam_cmp.camera.GetPerspectiveFov());
			  float persp_near_clip = cam_cmp.camera.GetPerspectiveNearClip();
			  float persp_far_clip = cam_cmp.camera.GetPerspectiveFarClip();

			  ImGui::Text("FOV");
			  ImGui::SameLine();
			  ImGui::DragFloat("##FOV", &persp_FOV);
			  ImGui::Text("Near clip");
			  ImGui::SameLine();
			  ImGui::DragFloat("##Near clip", &persp_near_clip);
			  ImGui::Text("Far clip");
			  ImGui::SameLine();
			  ImGui::DragFloat("##Far clip", &persp_far_clip);

			  cam_cmp.camera.SetPerspectiveFov(glm::radians(persp_FOV));
			  cam_cmp.camera.SetPerspectiveNearClip(persp_near_clip);
			  cam_cmp.camera.SetPerspectiveFarClip(persp_far_clip);
		  }

		  if (cam_cmp.camera.GetProjectionType() == ProjectionType::Orthographic)
		  {
			  float ortho_size = cam_cmp.camera.GetOrthographicSize();
			  float ortho_near_clip = cam_cmp.camera.GetOrthographicNearClip();
			  float ortho_far_clip = cam_cmp.camera.GetOrthographicFarClip();

			  ImGui::Text("Size");
			  ImGui::SameLine();
			  ImGui::DragFloat("##Size", &ortho_size);
			  ImGui::Text("Near clip");
			  ImGui::SameLine();
			  ImGui::DragFloat("##Near clip", &ortho_near_clip);
			  ImGui::Text("Far clip");
			  ImGui::SameLine();
			  ImGui::DragFloat("##Far clip", &ortho_far_clip);

			  cam_cmp.camera.SetOrthographicSize(ortho_size);
			  cam_cmp.camera.SetOrthographicNearClip(ortho_near_clip);
			  cam_cmp.camera.SetOrthographicFarClip(ortho_far_clip);

			  ImGui::Text("Fixed aspect ratio");
			  ImGui::SameLine();
			  ImGui::Checkbox("##Fixed aspect ratio", &cam_cmp.fix_aspect_ratio);

		  }
		});

		DrawComponent<SpriteRendererComponent>("SpriteRendererComponent", entity,
				[](SpriteRendererComponent& sprite_renderer)
				{
				  ImGui::Text("Color");
				  ImGui::SameLine();
				  ImGui::ColorEdit4("##Color", glm::value_ptr(sprite_renderer.color));

				  ImGui::Text("Texture");
				  if (sprite_renderer.texture)
				  {
					  if (ImGui::ImageButton("##Texture",
							  (ImTextureID)static_cast<uint64_t>(sprite_renderer.texture->GetRendererId()),
							  ImVec2(100, 100), { 1, 1 }, { 0, 0 }, { 0, 0, 0, 0 },
							  { sprite_renderer.color.r, sprite_renderer.color.g, sprite_renderer.color.b,
								sprite_renderer.color.a }))
					  {

					  };
				  }
				  else
				  {
					  ImGui::ColorButton("##Texture", *(ImVec4*)glm::value_ptr(sprite_renderer.color),
							  ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_DisplayHSV, ImVec2(100, 100));
				  }

				  if (ImGui::BeginDragDropTarget())
				  {
					  if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_TEXTURE"))
					  {
						  std::filesystem::path texture_path = (const char*)payload->Data;
						  if (!texture_path.empty())
						  {
							  sprite_renderer.texture = Texture2D::Create(texture_path.string());
						  }
					  }
					  ImGui::EndDragDropTarget();
				  }

				  ImGui::SameLine();
				  if (ImGui::Button("Delete Texture", { 100, 20 }))
				  {
					  sprite_renderer.texture = nullptr;
				  }

				});

		DrawComponent<CircleRendererComponent>("CircleRendererComponent", entity,
				[](auto& component)
				{
				  ImGui::Text("Color");
				  ImGui::SameLine();
				  ImGui::ColorEdit4("##Color", glm::value_ptr(component.color));
				  ImGui::Text("Thickness");
				  ImGui::SameLine();
				  ImGui::DragFloat("##Thickness", &component.thickness, 0.1f, 0.0f, 1.0f);
				  ImGui::Text("Fade");
				  ImGui::SameLine();
				  ImGui::DragFloat("##Fade", &component.fade, 0.001f, 0.0f, 1.0f);
				});

		DrawComponent<LineRendererComponent>("LineRendererComponent", entity,
				[](auto& component)
				{
				  ImGui::Text("Color");
				  ImGui::SameLine();
				  ImGui::ColorEdit4("##Color", glm::value_ptr(component.color));
				  ImGui::Text("Thickness");
				  ImGui::SameLine();
				  ImGui::DragFloat("##Thickness", &component.thickness, 0.1f, 0.0f, 1.0f);
				  //TODO List of vertex
				});

		DrawComponent<Rigidbody2DComponent>("Rigidbody2DComponent", entity, [](Rigidbody2DComponent& rb2d)
		{
		  const char* body_type_string[] = { "Static", "Dynamic", "Kinematic" };
		  const char* current_body_type_string = body_type_string[(int)rb2d.type];

		  ImGui::Text("Body Type");
		  ImGui::SameLine();
		  if (ImGui::BeginCombo("##Body Type", current_body_type_string))
		  {
			  for (int i = 0; i < 3; ++i)
			  {
				  bool is_selected = current_body_type_string == body_type_string[i];
				  if (ImGui::Selectable(body_type_string[i], is_selected))
				  {
					  current_body_type_string = body_type_string[i];
					  rb2d.type = (Rigidbody2DComponent::BodyType)i;
				  }

				  if (is_selected)
					  ImGui::SetItemDefaultFocus();
			  }

			  ImGui::EndCombo();
		  }
		  ImGui::Text("Fixed Rotation");
		  ImGui::SameLine();
		  ImGui::Checkbox("##Fixed Rotation", &rb2d.fixed_rotation);
		});

		DrawComponent<BoxCollider2DComponent>("BoxCollider2DComponent", entity, [](BoxCollider2DComponent& box2d)
		{
		  ImGui::Text("Offset");
		  ImGui::SameLine();
		  ImGui::DragFloat2("##Offset", glm::value_ptr(box2d.offset), 0.1f);

		  ImGui::Text("Size");
		  ImGui::SameLine();
		  ImGui::DragFloat2("##Size", glm::value_ptr(box2d.size));

		  ImGui::Text("Density");
		  ImGui::SameLine();
		  ImGui::DragFloat("##Density", &box2d.density, 0.01f, 0.0f, 1.0f);
		  ImGui::Text("Friction");
		  ImGui::SameLine();
		  ImGui::DragFloat("##Friction", &box2d.friction, 0.01f, 0.0f, 1.0f);
		  ImGui::Text("Restitution");
		  ImGui::SameLine();
		  ImGui::DragFloat("##Restitution", &box2d.restitution, 0.01f, 0.0f, 1.0f);
		  ImGui::Text("Restitution Threshold");
		  ImGui::SameLine();
		  ImGui::DragFloat("##Restitution Threshold", &box2d.restitution_threshold, 0.01f, 0.0f);
		});

		DrawComponent<CircleCollider2DComponent>("CircleCollider2DComponent", entity, [](auto& component)
		{
		  ImGui::Text("Offset");
		  ImGui::SameLine();
		  ImGui::DragFloat2("##Offset", glm::value_ptr(component.offset), 0.1f);
		  ImGui::Text("Radius");
		  ImGui::SameLine();
		  ImGui::DragFloat("##Radius", &component.radius);

		  ImGui::Text("Density");
		  ImGui::SameLine();
		  ImGui::DragFloat("##Density", &component.density, 0.01f, 0.0f, 1.0f);
		  ImGui::Text("Friction");
		  ImGui::SameLine();
		  ImGui::DragFloat("##Friction", &component.friction, 0.01f, 0.0f, 1.0f);
		  ImGui::Text("Restitution");
		  ImGui::SameLine();
		  ImGui::DragFloat("##Restitution", &component.restitution, 0.01f, 0.0f, 1.0f);
		  ImGui::Text("Restitution Threshold");
		  ImGui::SameLine();
		  ImGui::DragFloat("##Restitution Threshold", &component.restitution_threshold, 0.01f, 0.0f);
		});

	}

	Entity SceneHierarchyPanel::GetSelectedEntity()
	{
		return m_selection_entity;
	}

	void SceneHierarchyPanel::SetSelectedEntity(Entity& entity)
	{
		m_selection_entity = entity;
	}
}