#pragma once

#include <Engine/Core/Core.h>
namespace Engine
{
	class Texture
	{
	public:
		virtual ~Texture() = default;
		virtual uint32_t get_width() const = 0;
		virtual uint32_t get_height() const = 0;

		//
		virtual uint32_t GetRendererId() = 0;
		//
		virtual void Bind(uint32_t slot = 0) = 0;
		virtual void SetData(void* data, uint32_t size) = 0;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const std::string& path);
		static Ref<Texture2D> Create(uint32_t width, uint32_t height);
	};
} // namespace Engine