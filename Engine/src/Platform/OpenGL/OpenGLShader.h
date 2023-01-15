#pragma once
#include "Engine/Renderer/Shader.h"
#include "glm/glm.hpp"
#include <string>

namespace Engine
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(std::string vertex_src, std::string fragment_src);
		~OpenGLShader() override = default;

		virtual void Bind() const override;
		void UploadUniform(const std::string& name, float value);
		void UploadUniform(const std::string& name, const glm::mat4& value);
		void UploadUniform(const std::string& name, const glm::vec4& value);
		void UploadUniform(const std::string& name, int value);

	private:
		uint32_t m_renderer_id;
	};

} // namespace Engine