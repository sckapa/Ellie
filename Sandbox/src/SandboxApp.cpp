#include <Ellie.h>

class ExampleLayer : public Ellie::Layer
{
public:
	ExampleLayer() : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
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

		unsigned int indices[3] = { 0,1,2 };

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

		m_Shader.reset(new Ellie::Shader(vertexSrc, fragmentSrc));
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

		Ellie::Renderer::Submit(m_Shader, m_VertexArray);

		Ellie::Renderer::EndScene();
	}

private:
	Ellie::OrthographicCamera m_Camera;

	std::shared_ptr<Ellie::Shader> m_Shader;
	std::shared_ptr<Ellie::VertexArray> m_VertexArray;
	std::shared_ptr<Ellie::VertexBuffer> m_VertexBuffer;
	std::shared_ptr<Ellie::IndexBuffer> m_IndexBuffer;

	glm::vec3 m_CameraPosition = { 0.0f,0.0f,0.0f };
	float m_Rotation = 0.0f;

	float m_CameraSpeed = 5.0f;
	float m_RotationSpeed = 180.0f;
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