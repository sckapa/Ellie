#pragma once

#include "Window.h"
#include "LayerStack.h"
#include "Core.h"
#include "Events/Event.h"
#include "Ellie/Events/ApplicationEvent.h"


namespace Ellie{

	class ELLIE_API Application
	{
	public :
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	// To be defined in a CLIENT
	Application* CreateApplication();
}

