#include "pch.h"

#include "OpenGLFrameBuffer.h"
#include <glad/glad.h>
#include "Engine/Core/Core.h"

namespace Engine
{
	static const uint32_t s_max_framebuffer_size = 8192;

	namespace Utils
	{

		static GLenum TextureTarget(bool multisampled)
		{
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count)
		{
			glCreateTextures(TextureTarget(multisampled), count, outID);
		}

		static void BindTexture(bool multisampled, uint32_t id)
		{
			glBindTexture(TextureTarget(multisampled), id);
		}

		static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width,
				uint32_t height, int index)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
		}

		static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width,
				uint32_t height)
		{
			bool multisampled = samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
		}

		static bool IsDepthFormat(FrameBufferTextureFormat format)
		{
			switch (format)
			{
			case FrameBufferTextureFormat::DEPTH24STENCIL8:
				return true;
			}

			return false;
		}

		static GLenum FBTextureFormatToGL(FrameBufferTextureFormat format)
		{
			switch (format)
			{
			case FrameBufferTextureFormat::RGBA8:
				return GL_RGBA8;
			case FrameBufferTextureFormat::RED_INTEGER:
				return GL_RED_INTEGER;
			}

			ASSERT(false);
			return 0;
		}

	}
	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec)
			:m_specification{ spec }
	{

		for (auto spec : m_specification.attachment_specification.attachments)
		{
			if (!Utils::IsDepthFormat(spec.texture_format))
				m_color_attachment_specifications.emplace_back(spec);
			else
				m_DepthAttachmentSpecification = spec;
		}
		Invalidate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_renderer_id);
		glDeleteTextures(m_color_attachments.size(), m_color_attachments.data());
		glDeleteTextures(1, &m_depth_attachment);
	}

	void Engine::OpenGLFrameBuffer::Invalidate()
	{
		if (m_renderer_id != 0)
		{
			glDeleteFramebuffers(1, &m_renderer_id);
			glDeleteTextures(m_color_attachments.size(), m_color_attachments.data());
			glDeleteTextures(1, &m_depth_attachment);

			m_color_attachments.clear();
			m_depth_attachment = 0;
		}

		glCreateFramebuffers(1, &m_renderer_id);
		glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id);

		bool multisample = m_specification.samples > 1;

		// Attachments
		if (m_color_attachment_specifications.size())
		{
			m_color_attachments.resize(m_color_attachment_specifications.size());
			Utils::CreateTextures(multisample, m_color_attachments.data(), m_color_attachments.size());

			for (size_t i = 0; i < m_color_attachments.size(); i++)
			{
				Utils::BindTexture(multisample, m_color_attachments[i]);
				switch (m_color_attachment_specifications[i].texture_format)
				{
				case FrameBufferTextureFormat::RGBA8:
					Utils::AttachColorTexture(m_color_attachments[i], m_specification.samples, GL_RGBA8, GL_RGBA,
							m_specification.width, m_specification.height, i);
					break;
				case FrameBufferTextureFormat::RED_INTEGER:
					Utils::AttachColorTexture(m_color_attachments[i], m_specification.samples, GL_R32I, GL_RED_INTEGER,
							m_specification.width, m_specification.height, i);
					break;
				}
			}
		}

		if (m_DepthAttachmentSpecification.texture_format != FrameBufferTextureFormat::None)
		{
			Utils::CreateTextures(multisample, &m_depth_attachment, 1);
			Utils::BindTexture(multisample, m_depth_attachment);
			switch (m_DepthAttachmentSpecification.texture_format)
			{
			case FrameBufferTextureFormat::DEPTH24STENCIL8:
				Utils::AttachDepthTexture(m_depth_attachment, m_specification.samples, GL_DEPTH24_STENCIL8,
						GL_DEPTH_STENCIL_ATTACHMENT, m_specification.width, m_specification.height);
				break;
			}
		}

		if (m_color_attachments.size() > 1)
		{
			ASSERT((m_color_attachments.size() <= 4));
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2,
								  GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_color_attachments.size(), buffers);
		}
		else if (m_color_attachments.empty())
		{
			// Only depth-pass
			glDrawBuffer(GL_NONE);
		}

		CORE_ASSERT((glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE),
				"FrameBuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_renderer_id);
		glViewport(0, 0, m_specification.width, m_specification.height);
	}
	void OpenGLFrameBuffer::UnBind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Resize(uint32_t x, uint32_t y)
	{
		if (x <= 0 || y <= 0 || x > s_max_framebuffer_size || y > s_max_framebuffer_size)
		{
			CORE_ERROR("FrameBuffer is too small size. Don't resize so");
			return;
		}
		m_specification.width = x;
		m_specification.height = y;

		Invalidate();
	}
	int OpenGLFrameBuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		ASSERT((attachmentIndex < m_color_attachments.size()));

		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}
	void OpenGLFrameBuffer::ClearAttachment(uint32_t attachmentIndex, int value)
	{
		ASSERT((attachmentIndex < m_color_attachments.size()));

		auto& spec = m_color_attachment_specifications[attachmentIndex];
		glClearTexImage(m_color_attachments[attachmentIndex], 0,
				Utils::FBTextureFormatToGL(spec.texture_format), GL_INT, &value);
	}
}

