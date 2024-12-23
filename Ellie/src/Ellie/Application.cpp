#include "eepch.h"
#include "Application.h"

#include "Ellie/Events/ApplicationEvent.h"
#include "Ellie/Log.h"

namespace Ellie{

	Ellie::Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Ellie::Application::~Application()
	{
	}

	void Ellie::Application::Run()
	{
		while (m_Running)
		{
			m_Window->OnUpdate();
		}
	}
}
