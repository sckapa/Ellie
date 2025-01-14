#include <Ellie.h>
#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include "imgui.h"
#include "glm/gtc/type_ptr.hpp"

class ExampleLayer : public Ellie::Layer
{
public:
	ExampleLayer() : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_Position(glm::vec3(0.0f))
	{
		m_VertexArray.reset(Ellie::VertexArray::Create());

		float vertices[3 * 7] = {
		-0.5f, -0.5f, 0.0f, 0.8f, 0.3f, 0.2f, 1.0f,
		0.5f, -0.5f, 0.0f, 0.2f, 0.8f, 0.3f, 1.0f,
		0.0f, 0.5f, 0.0f, 0.3f, 0.2f, 0.8f, 1.0f
		};

		m_VertexBuffer.reset(Ellie::VertexBuffer::Create(vertices, sizeof(vertices)));

		Ellie::BufferLayout layout
		{
			{Ellie::ShaderDataType::Float3, "a_Position"},
			{Ellie::ShaderDataType::Float4, "a_Color"}
		};

		m_VertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffers(m_VertexBuffer);

		uint32_t indices[3] = { 0,1,2 };

		m_IndexBuffer.reset(Ellie::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		std::string vertexSrc = R"(
		#version 330 core

		layout(location = 0) in vec3 a_Position;
		layout(location = 1) in vec4 a_Color;

		uniform mat4 u_ViewProjection;

		out vec3 v_Position;
		out vec4 v_Color;

		void main()
		{
			v_Position = a_Position;
			v_Color = a_Color;
			gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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

		m_Triangle.reset(Ellie::Shader::Create(vertexSrc, fragmentSrc));

		//
		m_SqVertexArray.reset(Ellie::VertexArray::Create());

		float sqVertices[4 * 3] = {
		-0.5f, -0.5f, 0.0f, 
		0.5f, -0.5f, 0.0f, 
		0.5f, 0.5f, 0.0f, 
		-0.5f, 0.5f, 0.0f
		};

		m_SqVertexBuffer.reset(Ellie::VertexBuffer::Create(sqVertices, sizeof(sqVertices)));

		m_SqVertexBuffer->SetLayout({
				{Ellie::ShaderDataType::Float3, "a_Position"}
		});
		m_SqVertexArray->AddVertexBuffers(m_SqVertexBuffer);

		uint32_t sqIndices[6] = { 0,1,2,2,3,0 };

		m_SqIndexBuffer.reset(Ellie::IndexBuffer::Create(sqIndices, sizeof(sqIndices) / sizeof(uint32_t)));
		m_SqVertexArray->SetIndexBuffer(m_SqIndexBuffer);

		std::string sqVertexSrc = R"(
		#version 330 core

		layout(location = 0) in vec3 a_Position;

		uniform mat4 u_ViewProjection;
		uniform mat4 u_Transform;

		out vec3 v_Position;

		void main()
		{
			v_Position = a_Position;
			gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
		}
		)";

		std::string sqFragmentSrc = R"(
		#version 330 core

		layout(location = 0) out vec4 color;

		uniform vec4 u_Color;

		in vec3 v_Position;

		void main()
		{
			color = u_Color;
		}
		)";

		m_Square.reset(Ellie::Shader::Create(sqVertexSrc, sqFragmentSrc));
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Color Picker");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(sqColor));
		ImGui::End();
	}

	void OnEvent(Ellie::Event& e) override
	{
	}

	void OnUpdate(Ellie::Timestep ts) override
	{
		if (Ellie::Input::IsKeyPressed(EE_KEY_A))
		{
			m_Rotation += m_RotationSpeed * ts;
		}
		else if (Ellie::Input::IsKeyPressed(EE_KEY_D))
		{
			m_Rotation -= m_RotationSpeed * ts;
		}

		if (Ellie::Input::IsKeyPressed(EE_KEY_RIGHT))
		{
			m_CameraPosition.x += m_CameraSpeed * ts;
		}
		else if (Ellie::Input::IsKeyPressed(EE_KEY_LEFT))
		{
			m_CameraPosition.x -= m_CameraSpeed * ts;
		}

		if (Ellie::Input::IsKeyPressed(EE_KEY_DOWN))
		{
			m_CameraPosition.y -= m_CameraSpeed * ts;
		}
		else if (Ellie::Input::IsKeyPressed(EE_KEY_UP))
		{
			m_CameraPosition.y += m_CameraSpeed * ts;
		}

		Ellie::RenderCommands::Clear();
		Ellie::RenderCommands::SetClearColor({ 0.1f,0.1f,0.1f,1 });

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_Rotation);

		Ellie::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Ellie::OpenGLShader>(m_Square)->Bind();
		std::dynamic_pointer_cast<Ellie::OpenGLShader>(m_Square)->UploadUniformFloat4("u_Color", sqColor);

		for (int y = 0; y < 20; y++)
		{
			for (int i = 0; i < 20; i++)
			{
				glm::vec3 pos(y * 0.11f, i * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Ellie::Renderer::Submit(m_Square, m_SqVertexArray, transform);
			}
		}
		Ellie::Renderer::Submit(m_Triangle, m_VertexArray);

		Ellie::Renderer::EndScene();
	}

private:
	Ellie::OrthographicCamera m_Camera;

	Ellie::Ref<Ellie::Shader> m_Triangle;
	Ellie::Ref<Ellie::VertexArray> m_VertexArray;
	Ellie::Ref<Ellie::VertexBuffer> m_VertexBuffer;
	Ellie::Ref<Ellie::IndexBuffer> m_IndexBuffer;

	Ellie::Ref<Ellie::Shader> m_Square;
	Ellie::Ref<Ellie::VertexArray> m_SqVertexArray;
	Ellie::Ref<Ellie::VertexBuffer> m_SqVertexBuffer;
	Ellie::Ref<Ellie::IndexBuffer> m_SqIndexBuffer;

	glm::vec3 m_CameraPosition = { 0.0f,0.0f,0.0f };
	float m_Rotation = 0.0f;

	float m_CameraSpeed = 5.0f;
	float m_RotationSpeed = 180.0f;

	glm::vec3 m_Position;
	glm::vec4 sqColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};

class Sandbox : public Ellie::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}

};

Ellie::Application* Ellie::CreateApplication()
{
	return new Sandbox();
}