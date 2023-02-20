#pragma once

#include "Engine/Core/TimeStep.h"
#include "Engine/Scene/Entity.h"

namespace Engine
{
	template<typename... Args>
	class ScriptableEvent
	{
	public:
		using EventHandler = std::function<void(Args...)>;

		void Invoke(Args... args)
		{
			for (auto& subscriber : m_subscribers)
			{
				subscriber(args...);
			}
		}

		void operator+=(EventHandler subscriber)
		{
			m_subscribers.push_back(subscriber);
		}

		void operator-=(EventHandler subscriber)
		{
			m_subscribers.erase(std::remove(m_subscribers.begin(), m_subscribers.end(), subscriber),
					m_subscribers.end());
		}
	private:
		std::vector<EventHandler> m_subscribers;
	};

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
