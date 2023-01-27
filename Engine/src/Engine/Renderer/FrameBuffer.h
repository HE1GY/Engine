#pragma once

#include <Engine/Core/Core.h>
namespace Engine
{

	struct FrameBufferSpecification
	{
		uint32_t width, height;
		uint32_t sample = 1;

		bool swap_chain_target = false;
	};

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;
		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);

		virtual const FrameBufferSpecification& get_specification() const = 0;
		virtual uint32_t get_color_attachment_renderer_id() const = 0;

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual void Resize(uint32_t x, uint32_t y) = 0;
	};
}
