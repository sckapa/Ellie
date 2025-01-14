#include "eepch.h"
#include "RenderCommands.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Ellie {

	RendererAPI* RenderCommands::s_RendererAPI = new OpenGLRendererAPI;

}