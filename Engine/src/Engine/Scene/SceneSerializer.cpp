#include "pch.h"
#include "SceneSerializer.h"

#include <yaml-cpp/yaml.h>

#include "Entity.h"
#include "Components.h"

namespace YAML
{
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& values)
		{
			Node node;
			node.push_back(values.x);
			node.push_back(values.x);
			node.push_back(values.x);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& values)
		{
			if (!node.IsSequence() || node.size() != 3)
			{
				return false;
			}
			values.x = node[0].as<float>();
			values.y = node[1].as<float>();
			values.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& values)
		{
			Node node;
			node.push_back(values.x);
			node.push_back(values.y);
			node.push_back(values.z);
			node.push_back(values.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& values)
		{
			if (!node.IsSequence() || node.size() != 4)
			{
				return false;
			}
			values.x = node[0].as<float>();
			values.y = node[1].as<float>();
			values.z = node[2].as<float>();
			values.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Engine
{
	static YAML::Emitter& operator<<(YAML::Emitter& emitter, const glm::vec3& values)
	{
		emitter << YAML::Flow;
		emitter << YAML::BeginSeq << values.x << values.y << values.z << YAML::EndSeq;
		return emitter;
	}

	static YAML::Emitter& operator<<(YAML::Emitter& emitter, const glm::vec4& values)
	{
		emitter << YAML::Flow;
		emitter << YAML::BeginSeq << values.x << values.y << values.z << values.w << YAML::EndSeq;
		return emitter;
	}

	static void SerializeEntity(YAML::Emitter& emitter, Entity& entity)
	{
		emitter << YAML::BeginMap;

		emitter << YAML::Key << "EntityID" << YAML::Value << 12234124124;//TODO GUID

		if (entity.HasComponent<TagComponent>())
		{
			emitter << YAML::Key << "TagComponent";
			auto& component = entity.GetComponent<TagComponent>();

			emitter << YAML::BeginMap;
			emitter << YAML::Key << "Tag" << YAML::Value << component.tag;
			emitter << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>())
		{
			emitter << YAML::Key << "TransformComponent";
			auto& component = entity.GetComponent<TransformComponent>();

			emitter << YAML::BeginMap;
			emitter << YAML::Key << "Translation" << YAML::Value << component.translation;
			emitter << YAML::Key << "Rotation" << YAML::Value << component.rotation;
			emitter << YAML::Key << "Scale" << YAML::Value << component.scale;
			emitter << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			emitter << YAML::Key << "CameraComponent";
			auto& component = entity.GetComponent<CameraComponent>();

			emitter << YAML::BeginMap;

			emitter << YAML::Key << "Camera";
			emitter << YAML::BeginMap;
			emitter << YAML::Key << "Projection type" << YAML::Value << (int)component.camera.get_projection_type();
			emitter << YAML::Key << "Orthographic near clip" << YAML::Value
					<< component.camera.get_orthographic_near_clip();
			emitter << YAML::Key << "Orthographic far clip" << YAML::Value
					<< component.camera.get_orthographic_far_clip();
			emitter << YAML::Key << "Orthographic size" << YAML::Value << component.camera.get_orthographic_size();
			emitter << YAML::Key << "Perspective FOV" << YAML::Value << component.camera.get_perspective_FOV();
			emitter << YAML::Key << "Perspective near clip" << YAML::Value
					<< component.camera.get_perspective_near_clip();
			emitter << YAML::Key << "Perspective far clip" << YAML::Value
					<< component.camera.get_perspective_far_clip();
			emitter << YAML::EndMap;

			emitter << YAML::Key << "Fix aspect ration" << YAML::Value << component.fix_aspect_ratio;
			emitter << YAML::Key << "Primary" << YAML::Value << component.primary;

			emitter << YAML::EndMap;
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			emitter << YAML::Key << "SpriteRendererComponent";
			auto& component = entity.GetComponent<SpriteRendererComponent>();

			emitter << YAML::BeginMap;
			emitter << YAML::Key << "Color" << YAML::Value << component.color;
			emitter << YAML::EndMap;
		}

		emitter << YAML::EndMap;
	}

	SceneSerializer::SceneSerializer(Ref<Scene>& scene)
			:m_scene{ scene }
	{
	}

	void SceneSerializer::Serialize(const std::string& path)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value;

		out << YAML::BeginSeq;
		m_scene->m_registry.each([&](auto& entt)
		{
		  Entity entity = { entt, m_scene.get() };
		  if (!entity)
		  {
			  return;
		  }

		  SerializeEntity(out, entity);

		});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(path);
		if (fout.is_open())
		{
			fout << out.c_str();
			fout.close();
		}
		else
		{
			CORE_ASSERT(false, "Can't open file {0}", path);
		}

	}

	void SceneSerializer::SerializeRunTime(const std::string& path)
	{
		CORE_ASSERT(false, "Not Implemented");//TODO
	}

	bool SceneSerializer::Deserialize(const std::string& path)
	{
		std::ifstream fin(path, std::ios::in);
		if (!fin.is_open())
		{
			CORE_ASSERT(false, "Can't open file {0}", path);
			return false;
		}

		std::stringstream str_stream;
		str_stream << fin.rdbuf();
		YAML::Node data = YAML::Load(str_stream.str());

		if (!data["Scene"])
		{
			CORE_ERROR("There is not scene node to deserialize");
			return false;
		}

		std::string scene_name = data["Scene"].as<std::string>();
		CORE_TRACE("Deserializing scene {0}", scene_name);

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["EntityID"].as<uint64_t>(); //TODO

				std::string name;
				auto tag_comp_node = entity["TagComponent"];
				if (tag_comp_node)
				{
					name = tag_comp_node["Tag"].as<std::string>();
				}
				CORE_TRACE("Deserialize entity with id={0} name={1}", uuid, name);

				Entity deserialized_entity = m_scene->CreateEntity(name);//TODO with id

				auto transform_comp_node = entity["TransformComponent"];
				if (transform_comp_node)
				{
					auto& transform_comp = deserialized_entity.GetComponent<TransformComponent>();
					transform_comp.translation = transform_comp_node["Translation"].as<glm::vec3>();
					transform_comp.rotation = transform_comp_node["Rotation"].as<glm::vec3>();
					transform_comp.scale = transform_comp_node["Scale"].as<glm::vec3>();
				}

				auto camera_comp_node = entity["CameraComponent"];
				if (camera_comp_node)
				{
					auto camera_node = camera_comp_node["Camera"];

					auto& camera_comp = deserialized_entity.AddComponent<CameraComponent>();

					camera_comp.camera.set_projection_type(
							((SceneCamera::ProjectionType)camera_node["Projection type"].as<int>()));
					camera_comp.camera.set_orthographic(camera_node["Orthographic size"].as<float>(),
							camera_node["Orthographic near clip"].as<float>(),
							camera_node["Orthographic far clip"].as<float>());
					camera_comp.camera.set_perspective(camera_node["Perspective FOV"].as<float>(),
							camera_node["Perspective near clip"].as<float>(),
							camera_node["Perspective far clip"].as<float>());

					camera_comp.fix_aspect_ratio = camera_comp_node["Fix aspect ration"].as<bool>();
					camera_comp.primary = camera_comp_node["Primary"].as<bool>();

				}

				auto sprite_renderer_comp_node = entity["SpriteRendererComponent"];
				if (sprite_renderer_comp_node)
				{
					auto& sprite_renderer_comp = deserialized_entity.AddComponent<SpriteRendererComponent>();
					sprite_renderer_comp.color = sprite_renderer_comp_node["Color"].as<glm::vec4>();
				}

			}

		}

		return true;
	}

	bool SceneSerializer::DeserializeRunTime(const std::string& path)
	{
		CORE_ASSERT(false, "Not Implemented");//TODO
		return false;
	}
}