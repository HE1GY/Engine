#pragma once

namespace Engine
{

	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Int, Int2, Int3, Int4, Mat3, Mat4, Bool,
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type);

	struct BufferElement
	{
		std::string name;
		ShaderDataType type;
		bool normalized;
		uint32_t offset;
		uint32_t size;

		BufferElement(const std::string& name, ShaderDataType type, bool normalized);

		uint32_t GetComponentCount() const;

	};

	class BufferLayout
	{
	public:
		BufferLayout(std::initializer_list<BufferElement> layout);

		std::vector<BufferElement>::iterator begin()
		{
			return m_elements.begin();
		};
		std::vector<BufferElement>::iterator end()
		{
			return m_elements.end();
		};

		inline uint32_t get_stride()
		{
			return m_stride;
		}
	private:
		void CalculateOffsetAndStride();

		std::vector<BufferElement> m_elements;
		uint32_t m_stride;
	};

	class VertexBuffer
	{

	public:
		virtual ~VertexBuffer()
		{
		};

		static VertexBuffer* Create(void* data, uint32_t size);

		virtual void Bind() = 0;
		virtual void UnBind() = 0;
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer()
		{
		};

		static IndexBuffer* Create(uint32_t* data, uint32_t count);

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual inline uint32_t get_count() = 0;
	};
}; // namespace Engine