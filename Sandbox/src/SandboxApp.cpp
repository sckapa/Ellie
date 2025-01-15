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
		m_TextureVA.reset(Ellie::VertexArray::Create());

		float textureVertices[4 * 5] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
		};

		m_TextureVB.reset(Ellie::VertexBuffer::Create(textureVertices, sizeof(textureVertices)));

		m_TextureVB->SetLayout({
				{Ellie::ShaderDataType::Float3, "a_Position"},
				{Ellie::ShaderDataType::Float2, "a_TexCoords"}
			});
		m_TextureVA->AddVertexBuffers(m_TextureVB);

		uint32_t textureIndices[6] = { 0,1,2,2,3,0 };

		m_TextureIB.reset(Ellie::IndexBuffer::Create(textureIndices, sizeof(textureIndices) / sizeof(uint32_t)));
		m_TextureVA->SetIndexBuffer(m_TextureIB);

		std::string textureVertexSrc = R"(
		#version 330 core

		layout(location = 0) in vec3 a_Position;
		layout(location = 1) in vec2 a_TexCoords;

		uniform mat4 u_ViewProjection;
		uniform mat4 u_Transform;

		out vec3 v_Position;
		out vec2 v_TexCoords;

		void main()
		{
			v_Position = a_Position;
			v_TexCoords = a_TexCoords;
			gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
		}
		)";

		std::string textureFragmentSrc = R"(
		#version 330 core

		layout(location = 0) out vec4 color;

		uniform sampler2D u_Texture;

		in vec2 v_TexCoords;

		void main()
		{
			color = texture(u_Texture, v_TexCoords);
		}
		)";

		m_TextureShader.reset(Ellie::Shader::Create(textureVertexSrc, textureFragmentSrc));

		m_AbzTexture = Ellie::Texture2D::Create("assets/textures/abz.png");
		std::dynamic_pointer_cast<Ellie::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Ellie::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);

		//
		m_SqVertexArray.reset(Ellie::VertexArray::Create());

		float sqVertices[4 * 5] = {
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
		};

		m_SqVertexBuffer.reset(Ellie::VertexBuffer::Create(sqVertices, sizeof(sqVertices)));

		m_SqVertexBuffer->SetLayout({
				{Ellie::ShaderDataType::Float3, "a_Position"},
				{Ellie::ShaderDataType::Float2, "a_TexCoords"}
		});
		m_SqVertexArray->AddVertexBuffers(m_SqVertexBuffer);

		uint32_t sqIndices[6] = { 0,1,2,2,3,0 };

		m_SqIndexBuffer.reset(Ellie::IndexBuffer::Create(sqIndices, sizeof(sqIndices) / sizeof(uint32_t)));
		m_SqVertexArray->SetIndexBuffer(m_SqIndexBuffer);

		std::string sqVertexSrc = R"(
		#version 330 core

		layout(location = 0) in vec3 a_Position;
		layout(location = 1) in vec2 a_TexCoords;

		uniform mat4 u_ViewProjection;
		uniform mat4 u_Transform;

		out vec3 v_Position;
		out vec2 v_TexCoords;

		void main()
		{
			v_Position = a_Position;
			v_TexCoords = a_TexCoords;
			gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
		}
		)";

		std::string sqFragmentSrc = R"(
		#version 330 core

		layout(location = 0) out vec4 color;

		uniform sampler2D u_Texture;

		in vec2 v_TexCoords;

		void main()
		{
			color = texture(u_Texture, v_TexCoords);
		}
		)";

		m_Square.reset(Ellie::Shader::Create(sqVertexSrc, sqFragmentSrc));

		m_CheckerTexture = Ellie::Texture2D::Create("assets/textures/abc.png");
		m_TransparentTexture = Ellie::Texture2D::Create("assets/textures/fl.png");

		std::dynamic_pointer_cast<Ellie::OpenGLShader>(m_Square)->Bind();
		std::dynamic_pointer_cast<Ellie::OpenGLShader>(m_Square)->UploadUniformInt("u_Texture", 0);
	}

	virtual void OnImGuiRender() override
	{
		// ImGUI Code
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

		// Scene

		Ellie::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(1.5f));

		m_CheckerTexture->Bind();
		Ellie::Renderer::Submit(m_Square, m_SqVertexArray, scale);

		m_TransparentTexture->Bind();
		Ellie::Renderer::Submit(m_Square, m_SqVertexArray, scale);

		m_AbzTexture->Bind();
		Ellie::Renderer::Submit(m_TextureShader, m_TextureVA, glm::translate(glm::mat4(1.0f), glm::vec3(-1.3f, 0.6f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)));

		//Ellie::Renderer::Submit(m_Triangle, m_VertexArray);

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

	Ellie::Ref<Ellie::Shader> m_TextureShader;
	Ellie::Ref<Ellie::VertexArray> m_TextureVA;
	Ellie::Ref<Ellie::VertexBuffer> m_TextureVB;
	Ellie::Ref<Ellie::IndexBuffer> m_TextureIB;

	Ellie::Ref<Ellie::Texture2D> m_AbzTexture;
	Ellie::Ref<Ellie::Texture2D> m_CheckerTexture;
	Ellie::Ref<Ellie::Texture2D> m_TransparentTexture;

	glm::vec3 m_CameraPosition = { 0.0f,0.0f,0.0f };
	float m_Rotation = 0.0f;

	float m_CameraSpeed = 5.0f;
	float m_RotationSpeed = 180.0f;

	glm::vec3 m_Position;
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