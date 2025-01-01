#include "eepch.h"
#include "Buffers.h"

#include "Ellie/Platform/OpenGL/OpenGLBuffers.h"

namespace Ellie {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (RendererAPI::OpenGL)
		{
			case Ellie::RendererAPI::None:
			{
				EE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			}
			case Ellie::RendererAPI::OpenGL:
			{
				return new OpenGLVertexBuffer(vertices, size);
			}
		}

		EE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (RendererAPI::OpenGL)
		{
		case Ellie::RendererAPI::None:
		{
			EE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		}
		case Ellie::RendererAPI::OpenGL:
		{
			return new OpenGLIndexBuffer(indices, count);
		}
		}

		EE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}