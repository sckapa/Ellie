#include <Ellie.h>
#include <Ellie/Core/EntryPoint.h>

#include "EditorLayer.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"

namespace Ellie {

    EditorLayer::EditorLayer() : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
    {
    }

    EditorLayer::~EditorLayer()
    {
    }

    void EditorLayer::OnAttach()
    {
        m_Checker = Texture2D::Create("assets/textures/abc.png");

        FrameBufferSpecification spec;
        spec.width = 1280;
        spec.height = 720;
        m_FrameBuffer = FrameBuffer::Create(spec);

        m_ActiveScene = std::make_shared<Scene>();

        auto square = m_ActiveScene->CreateEntity();
        square.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f,0.0f,1.0f,1.0f });

        m_SquareEntity = square;

        m_CameraEntity = m_ActiveScene->CreateEntity("Camera Entity");
        m_CameraEntity.AddComponent<CameraComponent>();
    }

    void EditorLayer::OnDetach()
    {
    }

    void EditorLayer::OnEvent(Event& event)
    {
        m_CameraController.OnEvent(event);
    }

    void EditorLayer::OnUpdate(Timestep ts)
    {
        if (FrameBufferSpecification spec = m_FrameBuffer->GetFrameBufferSpec();
            m_ViewportSize.x > 0 && m_ViewportSize.y > 0
            (spec.width != m_ViewportSize.x || spec.height != m_ViewportSize.y))
        {
            m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
            m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
        }

        if (isViewportFocused)
        {
            m_CameraController.OnUpdate(ts);
        }
            
        // Renderer
        Renderer2D::ResetStats();

        m_FrameBuffer->Bind();

        RenderCommands::Clear();
        RenderCommands::SetClearColor({ 0.1f,0.1f,0.1f,1.0f });

        m_ActiveScene->OnUpdate(ts);

        m_FrameBuffer->Unbind();
    }

    void EditorLayer::OnImGuiRender()
    {
        // For Docking
        auto stats = Renderer2D::GetStatistics();

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
                if (ImGui::MenuItem("Exit")) { Application::Get().Close(); }
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        // Stats window
        ImGui::Begin("Stats");

        ImGui::Text("Draw calls : %d", stats.GetDrawCount());
        ImGui::Text("Quad count : %d", stats.GetQuadCount());

        auto& Color = m_SquareEntity.GetComponent<SpriteRendererComponent>().Color;
        ImGui::ColorEdit4("HI murgon", glm::value_ptr(Color));
        
        ImGui::Separator();
        auto& name = m_CameraEntity.GetComponent<TagComponent>().Tag;
        ImGui::Text("%s", name.c_str());

        ImGui::DragFloat3("Position", glm::value_ptr(m_CameraEntity.GetComponent<TransformComponent>().Transform[3]));
        ImGui::Separator();

        ImGui::End();

        // Viewport
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
        ImGui::Begin("Viewport");

        isViewportFocused = ImGui::IsWindowFocused();
        isViewportHovered = ImGui::IsWindowHovered();
        Application::Get().GetImGuiLayer()->SetBlocked(!isViewportFocused || !isViewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { viewportPanelSize.x,viewportPanelSize.y };

        uint32_t texID = m_FrameBuffer->GetColorAttachmentRendererID();
        ImGui::Image((ImTextureID)(uintptr_t)texID, ImVec2{ m_ViewportSize.x,m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));

        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::End();
    }
}