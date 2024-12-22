#include "Application.h"

#include "Ellie/Events/ApplicationEvent.h"
#include "Ellie/Log.h"

namespace Ellie{

	Ellie::Application::Application()
	{
	}

	Ellie::Application::~Application()
	{
	}

	void Ellie::Application::Run()
	{
		WindowResizeEvent resizeEvent(1280, 720);
		EE_INFO(resizeEvent);

		while (true)
		{

		}
	}
}
