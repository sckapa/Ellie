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
		glCreateBuffers(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffers(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		EE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer layout not set!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		const auto& layout = vertexBuffer->GetLayout();
		uint32_t index = 0;
		for (const auto& element : layout)
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
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}

}
