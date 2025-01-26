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
    m_FrameBuffer = Ellie::FrameBuffer::Create(spec);
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

    m_FrameBuffer->Bind();

	Ellie::RenderCommands::Clear();
	Ellie::RenderCommands::SetClearColor({0.1f,0.1f,0.1f,1.0f});

	Ellie::Renderer2D::BeginScene(m_CameraController.GetCamera());

	Ellie::Renderer2D::DrawQuad({0.0f, 0.0f, -0.1f}, {10.0f,10.0f}, m_SpriteSheet);

	Ellie::Renderer2D::EndScene();

    m_FrameBuffer->Unbind();
}

void Sandbox2D::OnImGuiRender()
{
	auto stats = Ellie::Renderer2D::GetStatistics();

    static bool dockspaceOpen = true;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit")) { Ellie::Application::Get().Close(); }
            ImGui::EndMenu();
        }
        
        ImGui::EndMenuBar();
    }

    ImGui::Begin("Stats");
    ImGui::Text("Draw calls : %d", stats.GetDrawCount());
    ImGui::Text("Quad count : %d", stats.GetQuadCount());

    uint32_t texID = m_FrameBuffer->GetColorAttachmentRendererID();
    ImGui::Image((ImTextureID)(uintptr_t)texID, ImVec2{ 1280.0f,720.0f }, ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();

    ImGui::End();
}
