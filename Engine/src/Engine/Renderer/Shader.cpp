#include "pch.h"

#include "Engine/Core.h"
#include "Shader.h"
#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLShader.h"

namespace Engine
{
	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertex_src, const std::string& fragment_src)
	{
		switch (RendererAPI::get_renderer_API())
		{

		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(name, vertex_src, fragment_src);

		default:
		CORE_ASSERT(false, "API = None");
			return nullptr;
		}

	}

	Ref<Shader> Shader::Create(const std::string& src_path)
	{
		switch (RendererAPI::get_renderer_API())
		{

		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLShader>(src_path);

		default:
		CORE_ASSERT(false, "API = None");
			return nullptr;
		}

	}
	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		const std::string& name = shader->get_name();
		Add(name, shader);
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		CORE_ASSERT((m_shaders.find(name) == m_shaders.end()), "Shader has already been added");
		m_shaders[name] = shader;
	}
	Ref<Shader> ShaderLibrary::Load(const std::string& src_path)
	{
		Ref<Shader> shader = Shader::Create(src_path);
		Add(shader);
		return shader;
	}
	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& src_path)
	{
		Ref<Shader> shader = Shader::Create(src_path);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		CORE_ASSERT((m_shaders.find(name) != m_shaders.end()), "Shader has not found");
		return m_shaders[name];
	}
} // namespace Engine