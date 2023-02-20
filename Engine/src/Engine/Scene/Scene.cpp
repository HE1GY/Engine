#include "pch.h"

#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Renderer/Camera.h"
#include "Engine/Renderer/RendererCommand.h"

namespace Engine
{

	Entity Scene::CreateEntity(const std::string& tag)
	{
		auto entity = Entity(m_registry.create(), this);
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>(tag);
		return entity;
	}

	void Scene::DestroyEntity(Entity& entity)
	{
		if (entity.HasComponent<NativeScriptComponent>()) //TODO fix Destroy script'
		{
			auto& nsc = entity.GetComponent<NativeScriptComponent>();
			nsc.instance->OnDestroy();
		}

		m_registry.destroy(entity);
	}

	void Scene::OnUpdateEditor(TimeStep ts, EditorCamera& camera)
	{
		Renderer2D::BeginScene(camera);

		auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

		for (auto entity : group)
		{
			auto [transform, sprite_renderer] = m_registry.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer2D::DrawSprite(transform.get_transformation(), sprite_renderer, (int32_t)entity);
		}

		Renderer2D::EndScene();
	}

	void Scene::OnUpdateRuntime(TimeStep ts)
	{
		//scripts
		{
			m_registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
			  if (!nsc.instance)
			  {
				  nsc.instance = nsc.InstantiateScript();
				  if (nsc.AfterInstantiateScript) nsc.AfterInstantiateScript(nsc.instance);
				  nsc.instance->m_entity = Entity(entity, this);
				  nsc.instance->m_scene = this;
				  nsc.instance->OnCreate();
			  }
			  nsc.instance->OnUpdate(ts);
			});
		}

		//physics
		m_registry.view<Box2DComponent, TransformComponent>().each([=](auto& box, auto& transform_cmp)
		{
		  box.position = glm::vec2(transform_cmp.translation);
		  box.size = glm::vec2(transform_cmp.scale);
		});
		m_registry.view<Circle2DComponent, TransformComponent>().each([=](auto& circle, auto& transform_cmp)
		{
		  circle.position = glm::vec2(transform_cmp.translation);
		  circle.radius = transform_cmp.scale.x / 2;
		});

		m_registry.view<Circle2DComponent, NativeScriptComponent>().each([=](auto& circle, auto& nsc_circle)
		{
		  m_registry.view<Box2DComponent, NativeScriptComponent>().each([=](auto& box, auto& nsc_box)
		  {
			float closest_x = glm::max(box.position.x - (box.size.x / 2),
					glm::min(box.position.x + (box.size.x / 2), circle.position.x));
			float closest_y = glm::max(box.position.y - (box.size.y / 2),
					glm::min(box.position.y + (box.size.y / 2), circle.position.y));

			float length = glm::length(circle.position - glm::vec2{ closest_x, closest_y });
			if (length < (circle.radius))
			{
				glm::vec2 normal = glm::normalize(circle.position - glm::vec2{ closest_x, closest_y });
				nsc_circle.instance->OnCollision2D(normal);
				nsc_box.instance->OnCollision2D(normal);
			}
		  });
		});






		//search camera
		Camera* main_camera{ nullptr };
		glm::mat4 cam_transform;

		auto view = m_registry.view<TransformComponent, CameraComponent>();
		for (auto entity : view)
		{
			auto [transform, cameraComponent] = view.get<TransformComponent, CameraComponent>(entity);

			if (cameraComponent.primary)
			{
				main_camera = &cameraComponent.camera;
				cam_transform = transform.get_transformation();
				break;
			}
		}

		//Render
		if (main_camera)
		{
			Renderer2D::BeginScene(*main_camera, cam_transform);

			auto group = m_registry.group<TransformComponent, SpriteRendererComponent>();

			for (auto entity : group)
			{
				auto [transform, sprite_renderer] = m_registry.get<TransformComponent, SpriteRendererComponent>(entity);
				if (!sprite_renderer.active)continue;

				Renderer2D::DrawSprite(transform.get_transformation(), sprite_renderer);
			}

			Renderer2D::EndScene();
		}
		else
		{
			WARN("Scene without camera");
		}

	}

	void Scene::OnViewResize(uint32_t width, uint32_t height)
	{
		m_viewport_width = width;
		m_viewport_height = height;

		auto view = m_registry.view<CameraComponent>();

		for (auto entity : view)
		{
			auto& camera = view.get<CameraComponent>(entity);
			if (!camera.fix_aspect_ratio)
			{
				camera.camera.SetViewport(width, height);
			}
		}
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = m_registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.primary)
			{
				return { entity, this };
			}
		}

		return { entt::null, nullptr };
	}
}