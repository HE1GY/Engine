#include "pch.h"

#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "Engine/Renderer/Renderer2D.h"
#include "Engine/Renderer/Camera.h"

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
		m_registry.destroy(entity);
	}

	void Scene::OnUpdate(TimeStep ts)
	{
		//scripts
		{
			m_registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
			  if (!nsc.instance)
			  {
				  nsc.instance = nsc.InstantiateScript();
				  nsc.instance->m_entity = Entity(entity, this);
				  nsc.instance->OnCreate();
			  }
			  nsc.instance->OnUpdate(ts);
			});
		}

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

		if (main_camera)
		{
			Renderer2D::BeginScene(*main_camera, cam_transform);

			auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

			for (auto entity : group)
			{
				auto [transform, sprite_renderer] = m_registry.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawQuad(transform.get_transformation(), sprite_renderer.color);
			}

			Renderer2D::EndScene();
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
				camera.camera.set_viewport(width, height);
			}
		}
	}

}