#include "eepch.h"
#include "Application.h"

#include "Ellie/Log.h"
#include "Input.h"
#include "glad/glad.h"


namespace Ellie{

#define BIND_EVENT_FN(x) std::bind (&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	static GLenum ShaderDataTypeToOpenGLDataType(ShaderDataType type)
	{
		switch (type)
		{
		case Ellie::ShaderDataType::Float:   return GL_FLOAT;
		case Ellie::ShaderDataType::Float2:  return GL_FLOAT;
		case Ellie::ShaderDataType::Float3:  return GL_FLOAT;
		case Ellie::ShaderDataType::Float4:  return GL_FLOAT;
		case Ellie::ShaderDataType::Mat3:    return GL_FLOAT;
		case Ellie::ShaderDataType::Mat4:    return GL_FLOAT;
		case Ellie::ShaderDataType::Int:     return GL_INT;
		case Ellie::ShaderDataType::Int2:    return GL_INT;
		case Ellie::ShaderDataType::Int3:    return GL_INT;
		case Ellie::ShaderDataType::Int4:    return GL_INT;
		case Ellie::ShaderDataType::Bool:    return GL_BOOL;
		}

		EE_ASSERT(false, "Invalid ShaderDataType!");
		return 0;
	}

	Ellie::Application::Application()
	{
		EE_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		float vertices[3 * 7] = {
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f
		};

		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		m_VertexBuffer->Bind();

		BufferLayout layout
		{
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"}
		};

		uint32_t index = 0;
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLDataType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset
			);
			index++;
		}

		unsigned int indices[3] = { 0,1,2 };

		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_IndexBuffer->Bind();

		std::string vertexSrc = R"(
		#version 330 core

		layout(location = 0) in vec3 a_Position;
		layout(location = 1) in vec4 a_Color;

		out vec3 v_Position;
		out vec4 v_Color;

		void main()
		{
			v_Position = a_Position;
			v_Color = a_Color;
			gl_Position = vec4(a_Position, 1.0);
		}
		)";

		std::string fragmentSrc = R"(
		#version 330 core

		layout(location = 0) out vec4 color;

		in vec3 v_Position;
		in vec4 v_Color;

		void main()
		{
			color = v_Color;
		}
		)";

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
	}

	Ellie::Application::~Application()
	{
	}

	void Ellie::Application::Run()
	{
		while (m_Running)
		{
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
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

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		//EE_CORE_TRACE(e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.IsHandled())
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
		m_Running = false;

		return true;
	}
}
