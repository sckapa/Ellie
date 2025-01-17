#include <Ellie.h>
#include <Ellie/Core/EntryPoint.h>

#include "Sandbox2D.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"


Sandbox2D::Sandbox2D() : Layer("Sandbox2D"), m_CameraController(1280.0f/720.0f)
{
}

Sandbox2D::~Sandbox2D()
{
}

void Sandbox2D::OnAttach()
{
	// Geometry
	m_SqVertexArray = Ellie::VertexArray::Create();

	float vertices[3 * 4]
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f
	};

	auto layout = Ellie::BufferLayout({
		{Ellie::ShaderDataType::Float3, "a_Position"}
	});

	std::shared_ptr<Ellie::VertexBuffer> sqVB;
	sqVB.reset(Ellie::VertexBuffer::Create(vertices, sizeof(vertices)));

	sqVB->SetLayout(layout);
	m_SqVertexArray->AddVertexBuffers(sqVB);

	uint32_t indices[6]{ 0,1,2,2,3,0 };

	std::shared_ptr<Ellie::IndexBuffer> sqIB;
	sqIB.reset(Ellie::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

	m_SqVertexArray->SetIndexBuffer(sqIB);

	// Shader
	m_FlatColorShader = Ellie::Shader::Create("assets/shaders/Texture.glsl");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnEvent(Ellie::Event& event)
{
	m_CameraController.OnEvent(event);
}

void Sandbox2D::OnUpdate(Ellie::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	// Renderer
	Ellie::RenderCommands::Clear();
	Ellie::RenderCommands::SetClearColor({0.1f,0.1f,0.1f,1.0f});

	Ellie::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<Ellie::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<Ellie::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_Color);

	Ellie::Renderer::Submit(m_FlatColorShader, m_SqVertexArray);

	Ellie::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Color");
	ImGui::ColorPicker4("Col", glm::value_ptr(m_Color));
	ImGui::End();
}
