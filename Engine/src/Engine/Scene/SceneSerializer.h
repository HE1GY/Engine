#pragma once

#include "Scene.h"
#include "Engine/Core/Core.h"

namespace Engine
{
	class SceneSerializer
	{
	public:
		SceneSerializer(Ref<Scene>& scene);

		void Serialize(const std::string& path);
		void SerializeRunTime(const std::string& path);

		bool Deserialize(const std::string& path);
		bool DeserializeRunTime(const std::string& path);
	private:
		Ref<Scene> m_scene;
	};
}
