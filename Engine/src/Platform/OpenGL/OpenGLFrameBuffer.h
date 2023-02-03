#pragma once
#include <Engine/Renderer/FrameBuffer.h>

namespace Engine
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		virtual ~OpenGLFrameBuffer() override;

		virtual const FrameBufferSpecification& GetSpecification() const override
		{
			return m_specification;
		};
		virtual uint32_t GetColorAttachmentRendererId(uint32_t index) const override
		{
			return m_color_attachments[index];
		}

		virtual void Bind() override;
		virtual void UnBind() override;

		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override
		{
			ASSERT((index < m_color_attachments.size()));
			return m_color_attachments[index];
		};

		virtual void Resize(uint32_t x, uint32_t y) override;

	private:
		void Invalidate();

		uint32_t m_renderer_id{ 0 };

		std::vector<uint32_t> m_color_attachments;
		std::vector<FrameBufferTextureSpecification> m_color_attachment_specifications;

		uint32_t m_depth_attachment{ 0 };
		FrameBufferTextureSpecification m_DepthAttachmentSpecification = FrameBufferTextureFormat::None;

		FrameBufferSpecification m_specification;

	};

}

