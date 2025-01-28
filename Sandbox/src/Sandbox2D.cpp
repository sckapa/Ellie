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
	m_Checker = Ellie::Texture2D::Create("assets/textures/abc.png");
	m_SpriteSheet = Ellie::Texture2D::Create("assets/game/textures/RPGpack_sheet_2X.png");

    Ellie::FrameBufferSpecification spec;
    spec.width = 1280;
    spec.height = 720;
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
	Ellie::Renderer2D::ResetStats();

	Ellie::RenderCommands::Clear();
	Ellie::RenderCommands::SetClearColor({0.1f,0.1f,0.1f,1.0f});

	Ellie::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Ellie::Renderer2D::DrawQuad({0.0f, 0.0f, -0.1f}, {10.0f,10.0f}, m_Checker);

	Ellie::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	auto stats = Ellie::Renderer2D::GetStatistics();

    ImGui::Begin("Stats");
    ImGui::Text("Draw calls : %d", stats.GetDrawCount());
    ImGui::Text("Quad count : %d", stats.GetQuadCount());

    ImGui::End();
}
