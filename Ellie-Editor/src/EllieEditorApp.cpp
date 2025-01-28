#include <Ellie.h>
#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "EditorLayer.h"

namespace Ellie {

	class EllieEditor : public Application
	{
	public:
		EllieEditor() : Application("Ellie Editor")
		{
			PushLayer(new EditorLayer());
		}

		~EllieEditor()
		{

		}

	};

	Application* CreateApplication()
	{
		return new EllieEditor();
	}
}