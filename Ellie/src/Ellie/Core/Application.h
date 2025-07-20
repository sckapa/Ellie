#pragma once

#include "Window.h"
#include "LayerStack.h"
#include "Core.h"
#include "Ellie/Events/Event.h"
#include "Ellie/Events/ApplicationEvent.h"
#include "Ellie/ImGui/ImGuiLayer.h"

#include "Ellie/Core/Timestep.h"

#include "Ellie/Renderer/Shader.h"
#include "Ellie/Renderer/Buffers.h"
#include "Ellie/Renderer/VertexArray.h"

#include "Ellie/Renderer/OrthographicCamera.h"

namespace Ellie{

	class ELLIE_API Application
	{
	public :
		Application(std::string name = "Ellie App");
		virtual ~Application();

		void Run();
		void Close();

		void SubmitToMainThreadQueue(const std::function<void()>& func);

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; } // Remove later, used for blocking events in editorlayer
	private:
		void ExecuteMainThreadFunctionQueue();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		ImGuiLayer* m_ImGuiLayer;
		std::unique_ptr<Window> m_Window;

		bool m_Running = true;
		bool m_Minimized = false;

		LayerStack m_LayerStack;

		static Application* s_Instance;

		std::mutex m_MainThreadQueueMutex;
		std::vector<std::function<void()>> m_MainThreadFunctionQueue;

		float m_LastFrameTime = 0.0f;
	};

	// To be defined in a CLIENT
	Application* CreateApplication();
}

