#include "eepch.h"
#include "Buffers.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLBuffers.h"

namespace Ellie {

	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (RendererAPI::API::OpenGL)
		{
			case Ellie::RendererAPI::API::None:
			{
				EE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
				return nullptr;
			}
			case Ellie::RendererAPI::API::OpenGL:
			{
				return new OpenGLVertexBuffer(vertices, size);
			}
		}

		EE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (RendererAPI::API::OpenGL)
		{
		case Ellie::RendererAPI::API::None:
		{
			EE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		}
		case Ellie::RendererAPI::API::OpenGL:
		{
			return new OpenGLIndexBuffer(indices, count);
		}
		}

		EE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}