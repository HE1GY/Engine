#include "pch.h"

#include "OpenGLTexture2D.h"

#include <glad/glad.h>

#include "stb_image/stb_image.h"

namespace Engine
{
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
			:m_path{ path }
	{

		stbi_set_flip_vertically_on_load(1);
		int width, height, channels;
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

		if (!data)
		{
			CORE_ERROR("Failed to load image!");
			const char* reason = stbi_failure_reason();
			CORE_ERROR(reason);
			CORE_ASSERT(false, "Path: {0}", path);
		}

		m_width = width;
		m_height = height;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_renderer_id);
		glTextureStorage2D(m_renderer_id, 1, GL_RGB8, m_width, m_height);

		glTextureParameteri(m_renderer_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_renderer_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_renderer_id, 0, 0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D() noexcept
	{
		glDeleteTextures(1, &m_renderer_id);
	}
	void OpenGLTexture2D::Bind(uint32_t slot)
	{
		glBindTextureUnit(slot, m_renderer_id);
	}

}
