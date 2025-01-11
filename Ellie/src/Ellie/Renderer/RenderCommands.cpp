#include "eepch.h"
#include "RenderCommands.h"
#include "Ellie/Platform/OpenGL/OpenGLRendererAPI.h"

namespace Ellie {

	RendererAPI* RenderCommands::s_RendererAPI = new OpenGLRendererAPI;

}