#pragma once

#include "Engine/Core/TimeStep.h"
#include "Engine/Scene/Entity.h"

namespace Engine
{
	class ScriptableEntity
	{
	public:
		virtual void OnCreate()
		{
		};
		virtual void OnUpdate(TimeStep ts)
		{
		};
		virtual void OnDestroy()
		{
		};

		template<typename T>
		T& GetComponent()
		{
			return m_entity.GetComponent<T>();
		}

	private:
		Entity m_entity;

		friend class Scene;
	};
}
