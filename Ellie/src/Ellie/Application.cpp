#include "eepch.h"
#include "Application.h"

#include "Ellie/Log.h"

namespace Ellie{

#define BIND_EVENT_FN(x) std::bind (&Application::x, this, std::placeholders::_1)

	Ellie::Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));
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

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		EE_TRACE(e);
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;

		return true;
	}
}
