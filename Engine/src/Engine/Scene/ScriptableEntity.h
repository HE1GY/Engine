#pragma once

#include "Engine/Core/TimeStep.h"
#include "Engine/Scene/Entity.h"

namespace Engine
{

	class ScriptableEntity
	{
	public:

		virtual ~ScriptableEntity()
		{
		}
		virtual void OnCreate()
		{
		};
		virtual void OnUpdate(TimeStep ts)
		{
		};
		virtual void OnDestroy()
		{
		};

		virtual void OnCollision2D(glm::vec2& normal)
		{
		};

		template<typename T>
		T& GetComponent()
		{
			return m_entity.GetComponent<T>();
		}

		void Destroy()
		{
			m_scene->DestroyEntity(m_entity);
		}

		Entity GetMainCamera() const
		{
			return m_scene->GetPrimaryCameraEntity();
		}

	private:
		Entity m_entity;
		Scene* m_scene;
		friend class Scene;
	};

}
