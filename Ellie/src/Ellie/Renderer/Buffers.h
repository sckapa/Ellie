#pragma once

#include "Renderer.h"

namespace Ellie {

	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case Ellie::ShaderDataType::Float:     return 4;
			case Ellie::ShaderDataType::Float2:    return 4 * 2;
			case Ellie::ShaderDataType::Float3:    return 4 * 3;
			case Ellie::ShaderDataType::Float4:    return 4 * 4;
			case Ellie::ShaderDataType::Mat3:      return 4 * 3 * 3;
			case Ellie::ShaderDataType::Mat4:      return 4 * 4 * 4;
			case Ellie::ShaderDataType::Int:       return 4;
			case Ellie::ShaderDataType::Int2:      return 4 * 2;
			case Ellie::ShaderDataType::Int3:      return 4 * 3;
			case Ellie::ShaderDataType::Int4:      return 4 * 4;
			case Ellie::ShaderDataType::Bool:      return 1;
		}

		EE_ASSERT(false, "Invalid ShaderDataType!");
		return 0;
	}

	struct BufferElement
	{
		std::string Name;
		uint32_t Size;
		uint32_t Offset;
		ShaderDataType Type;
		bool Normalized;

		BufferElement(ShaderDataType type, const std::string name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{
		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
				case Ellie::ShaderDataType::Float:     return 1;
				case Ellie::ShaderDataType::Float2:    return 2;
				case Ellie::ShaderDataType::Float3:    return 3;
				case Ellie::ShaderDataType::Float4:    return 4;
				case Ellie::ShaderDataType::Mat3:      return 3 * 3;
				case Ellie::ShaderDataType::Mat4:      return 4 * 4;
				case Ellie::ShaderDataType::Int:       return 1;
				case Ellie::ShaderDataType::Int2:      return 2;
				case Ellie::ShaderDataType::Int3:      return 3;
				case Ellie::ShaderDataType::Int4:      return 4;
				case Ellie::ShaderDataType::Bool:      return 1;
			}

			EE_ASSERT(false, "Invalid ShaderDataType!");
			return 0;
		}

		BufferElement(){}
	};

	class BufferLayout
	{
	public:
		BufferLayout(){}

		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements)
		{
			CalculateOffsetAndStride();
		}

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

		const std::vector<BufferElement> GetElements() const { return m_Elements; }
		const uint32_t GetStride() const { return m_Stride; }

	private:
		void CalculateOffsetAndStride()
		{
			uint32_t offset = 0;
			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}

		uint32_t m_Stride = 0;
		std::vector<BufferElement> m_Elements;
	};

	class VertexBuffer
	{
	public:
		~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetLatout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLatout() const = 0;

		static VertexBuffer* Create(float* vertices, uint32_t size);
	};

	class IndexBuffer
	{
	public:
		~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() = 0;

		static IndexBuffer* Create(uint32_t* indices, uint32_t count);
	};

}

