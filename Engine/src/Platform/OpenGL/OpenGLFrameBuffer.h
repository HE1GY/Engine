#pragma once
#include <Engine/Renderer/FrameBuffer.h>

namespace Engine
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		~OpenGLFrameBuffer();

		virtual const FrameBufferSpecification& get_specification() const override
		{
			return m_specification;
		};
		virtual uint32_t get_color_attachment_renderer_id() const override
		{
			return m_color_attachment;
		}

		virtual void Bind() override;
		virtual void UnBind() override;

	private:
		void Invalidate();

		uint32_t m_renderer_id;

		uint32_t m_color_attachment;
		uint32_t m_depth_attachment;

		FrameBufferSpecification m_specification;
	};

}

