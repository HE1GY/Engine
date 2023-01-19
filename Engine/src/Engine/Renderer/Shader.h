#pragma once

#include "Engine/Core/Core.h"
#include "glm/glm.hpp"

namespace Engine
{
	class Shader
	{
	public:
		static Ref<Shader>
		Create(const std::string& name, const std::string& vertex_src, const std::string& fragment_src);
		static Ref<Shader> Create(const std::string& src_path);
		virtual ~Shader() = default;
		virtual void Bind() const = 0;
		virtual const std::string& get_name() const = 0;

		virtual void SetMat4(const std::string& name, const glm::mat4& mat) = 0;
		virtual void SetVec4(const std::string& name, const glm::vec4& vec) = 0;
		virtual void SetInt(const std::string& name, const int value) = 0;
	};

	class ShaderLibrary
	{
	public:
		void Add(const Ref<Shader>& shader);
		void Add(const std::string& name, const Ref<Shader>& shader);
		Ref<Shader> Load(const std::string& src_path);
		Ref<Shader> Load(const std::string& name, const std::string& src_path);
		Ref<Shader> Get(const std::string& name);

	private:
		std::unordered_map<std::string, Ref<Shader>> m_shaders;
	};
} // namespace Engine