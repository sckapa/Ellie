#include "eepch.h"
#include "OpenGLVertexArray.h"

#include "glad/glad.h"

namespace Ellie{

	static GLenum ShaderDataTypeToOpenGLDataType(ShaderDataType type)
	{
		switch (type)
		{
		case Ellie::ShaderDataType::Float:   return GL_FLOAT;
		case Ellie::ShaderDataType::Float2:  return GL_FLOAT;
		case Ellie::ShaderDataType::Float3:  return GL_FLOAT;
		case Ellie::ShaderDataType::Float4:  return GL_FLOAT;
		case Ellie::ShaderDataType::Mat3:    return GL_FLOAT;
		case Ellie::ShaderDataType::Mat4:    return GL_FLOAT;
		case Ellie::ShaderDataType::Int:     return GL_INT;
		case Ellie::ShaderDataType::Int2:    return GL_INT;
		case Ellie::ShaderDataType::Int3:    return GL_INT;
		case Ellie::ShaderDataType::Int4:    return GL_INT;
		case Ellie::ShaderDataType::Bool:    return GL_BOOL;
		}

		EE_ASSERT(false, "Invalid ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffers(const Ref<VertexBuffer>& vertexBuffer)
	{
		EE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer layout not set!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();
		uint32_t index = 0;
		for (const auto& element : layout)
		{
			switch (element.Type)
			{
				case ShaderDataType::Float:
				case ShaderDataType::Float2:
				case ShaderDataType::Float3:
				case ShaderDataType::Float4:
				{
					glEnableVertexAttribArray(index);
					glVertexAttribPointer(
						index,
						element.GetComponentCount(),
						ShaderDataTypeToOpenGLDataType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)element.Offset
					);
					index++;
					break;
				}
				case ShaderDataType::Int:
				case ShaderDataType::Int2:
				case ShaderDataType::Int3:
				case ShaderDataType::Int4:
				case ShaderDataType::Bool:
				{
					glEnableVertexAttribArray(index);
					glVertexAttribIPointer(
						index,
						element.GetComponentCount(),
						ShaderDataTypeToOpenGLDataType(element.Type),
						layout.GetStride(),
						(const void*)element.Offset
					);
					index++;
					break;
				}
				case ShaderDataType::Mat3:
				case ShaderDataType::Mat4:
				default: EE_CORE_ASSERT(false, "Unknown ShaderDataType");
			}
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

}
