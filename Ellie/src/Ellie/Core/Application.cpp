#include "eepch.h"
#include "Application.h"

#include "Ellie/Core/Log.h"
#include "Input.h"
#include <GLFW/glfw3.h>

#include "Ellie/Renderer/Renderer.h"
#include "Ellie/Scripting/ScriptEngine.h"

namespace Ellie{

	Application* Application::s_Instance = nullptr;

	Application::Application(std::string name)
	{
		EE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create(WindowProps(name)));
		m_Window->SetEventCallback(EE_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();
		ScriptEngine::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		ScriptEngine::Shutdown();
	}

	void Application::Run()
	{
		while (m_Running)
		{
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			ExecuteMainThreadFunctionQueue();

			if(!m_Minimized)
			{
				for (Layer* layer : m_LayerStack)
				{
					layer->OnUpdate(timestep);
				}
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
			{
				layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::ExecuteMainThreadFunctionQueue()
	{
		for (auto& func : m_MainThreadFunctionQueue)
		{
			func();
		}
		m_MainThreadFunctionQueue.clear();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::SubmitToMainThreadQueue(const std::function<void()>& func)
	{
		std::scoped_lock<std::mutex> lock(m_MainThreadQueueMutex);

		m_MainThreadFunctionQueue.emplace_back(func);
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(EE_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(EE_BIND_EVENT_FN(Application::OnWindowResize));

		//EE_CORE_TRACE(e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
			{
				break;
			}
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		Close();

		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
		}

		m_Minimized = false;

		Renderer::WindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
}
