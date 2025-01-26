#include "eepch.h"
#include "FrameBuffer.h"
#include "Ellie/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLFrameBuffer.h"


namespace Ellie {

	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
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
			return std::make_shared<OpenGLFrameBuffer>(spec);
		}
		}

		EE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}