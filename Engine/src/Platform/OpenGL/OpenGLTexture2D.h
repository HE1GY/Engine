#pragma once

#include <Engine/Renderer/Texture.h>
namespace Engine
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D() override;

		uint32_t get_width() const override
		{
			return m_width;
		};
		uint32_t get_height() const override
		{
			return m_height;
		};

		void Bind(uint32_t slot = 0) override;

		std::string m_path;
		uint32_t m_height;
		uint32_t m_width;
		uint32_t m_renderer_id;
	};
}


