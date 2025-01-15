#include "eepch.h"
#include "Texture.h"
#include "Ellie/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture2D.h"

namespace Ellie {

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
		{
			EE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		}
		case RendererAPI::API::OpenGL:
		{
			return std::make_shared<OpenGLTexture2D>(path);
		}
		}

		EE_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}