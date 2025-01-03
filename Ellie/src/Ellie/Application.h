#pragma once

#include "Window.h"
#include "LayerStack.h"
#include "Core.h"
#include "Events/Event.h"
#include "Ellie/Events/ApplicationEvent.h"
#include "ImGui/ImGuiLayer.h"

#include "Renderer/Shader.h"
#include "Ellie/Renderer/Buffers.h"
#include "Ellie/Renderer/VertexArray.h"

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

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		ImGuiLayer* m_ImGuiLayer;
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;

		static Application* s_Instance;

		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};

	// To be defined in a CLIENT
	Application* CreateApplication();
}

