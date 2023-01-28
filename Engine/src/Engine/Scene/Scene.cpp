#include "pch.h"

#include "Scene.h"
#include "Entity.h"
#include "Components.h"
#include "Engine/Renderer/Renderer2D.h"

namespace Engine
{

	Entity Scene::CreateEntity(const std::string& tag)
	{
		auto entity = Entity(m_registry.create(), this);
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>(tag);
		return entity;
	}
	void Scene::OnUpdate(TimeStep ts)
	{
		auto group = m_registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

		for (auto entity : group)
		{
			auto [transform, sprite_renderer] = m_registry.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer2D::DrawQuad(transform, sprite_renderer.color);
		}
	}
}