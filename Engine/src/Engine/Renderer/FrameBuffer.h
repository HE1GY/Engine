#pragma once

#include <Engine/Core/Core.h>
namespace Engine
{
	enum class FrameBufferTextureFormat
	{
		None = 0,

		//Color
		RGBA8,
		RED_INTEGER,

		//Depth/Stencil
		DEPTH24STENCIL8,

		//Default
		Depth = DEPTH24STENCIL8,

	};

	struct FrameBufferTextureSpecification
	{
		FrameBufferTextureSpecification() = default;
		FrameBufferTextureSpecification(FrameBufferTextureFormat texture_format)
				:texture_format{ texture_format }
		{
		}
		FrameBufferTextureFormat texture_format;
		//TODO: filtering/wrap
	};

	struct FrameBufferAttachmentSpecification
	{
		FrameBufferAttachmentSpecification() = default;
		FrameBufferAttachmentSpecification(std::initializer_list<FrameBufferTextureSpecification> attachments)
				:attachments{ attachments }
		{
		}

		std::vector<FrameBufferTextureSpecification> attachments;
	};

	struct FrameBufferSpecification
	{
		FrameBufferAttachmentSpecification attachment_specification;
		uint32_t width, height;
		uint32_t samples = 1;

		bool swap_chain_target = false;
	};

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;
		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);

		virtual const FrameBufferSpecification& GetSpecification() const = 0;
		virtual uint32_t GetColorAttachmentRendererId(uint32_t index = 0) const = 0;

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;

		virtual void Resize(uint32_t x, uint32_t y) = 0;
	};
}
