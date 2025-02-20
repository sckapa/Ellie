#include <Ellie.h>
#include <Ellie/Core/EntryPoint.h>

#include "EditorLayer.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "Ellie/Scene/ScriptableEntity.h"
#include "Ellie/Scene/SceneSerializer.h"

#include "Ellie/Utils/PlatformUtils.h"
#include "ImGuizmo.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/matrix_decompose.hpp"
#include <glm/gtc/quaternion.hpp>

namespace Ellie {

    ImGuizmo::OPERATION m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;

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

        m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

#if 0

        auto square = m_ActiveScene->CreateEntity("Blue Square");
        square.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.0f,0.0f,1.0f,1.0f });

        m_SquareEntity = square;

        m_CameraEntity = m_ActiveScene->CreateEntity("Camera Entity");
        m_CameraEntity.AddComponent<CameraComponent>();

        class CameraController : public ScriptableEntity
        {
        public:
            void OnCreate()
            {
            }

            void OnDestroy()
            {
            }

            void OnUpdate(Timestep ts)
            {
                auto& tr = GetComponent<TransformComponent>().GetTransform();
                if (Input::IsKeyPressed(EE_KEY_A))
                {
                    tr[3][0] -= 5.0f * ts;
                }
                if (Input::IsKeyPressed(EE_KEY_D))
                {
                    tr[3][0] += 5.0f * ts;
                }
                if (Input::IsKeyPressed(EE_KEY_W))
                {
                    tr[3][1] += 5.0f * ts;
                }
                if (Input::IsKeyPressed(EE_KEY_S))
                {
                    tr[3][1] -= 5.0f * ts;
                }
            }
        };

        m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

#endif
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }

    void EditorLayer::OnDetach()
    {
    }

    void EditorLayer::OnUpdate(Timestep ts)
    {
        if (FrameBufferSpecification spec = m_FrameBuffer->GetFrameBufferSpec();
            m_ViewportSize.x > 0 && m_ViewportSize.y > 0
            (spec.width != m_ViewportSize.x || spec.height != m_ViewportSize.y))
        {
            m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
            m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
            m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
        }


        if (isViewportFocused)
        {
            m_CameraController.OnUpdate(ts);
            m_EditorCamera.OnUpdate(ts);
        }
            
        // Renderer
        Renderer2D::ResetStats();

        m_FrameBuffer->Bind();

        RenderCommands::Clear();
        RenderCommands::SetClearColor({ 0.1f,0.1f,0.1f,1.0f });

        m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);

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
        ImGuiStyle& style = ImGui::GetStyle();
        float minWinSizeX = style.WindowMinSize.x;
        style.WindowMinSize.x = 370.0f;
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        style.WindowMinSize.x = minWinSizeX;

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New", "Ctrl+N"))
                {
                    NewScene();
                }

                if (ImGui::MenuItem("Open...", "Ctrl+O"))
                {
                    OpenScene();
                }

                if (ImGui::MenuItem("SaveAs...", "Crtl+Shift+S"))
                {
                    SaveSceneAs();
                }

                if (ImGui::MenuItem("Exit")) { Application::Get().Close(); }
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        // Stats window
        ImGui::Begin("Stats");

        ImGui::Text("Draw calls : %d", stats.GetDrawCount());
        ImGui::Text("Quad count : %d", stats.GetQuadCount());

        ImGui::End();

        // Hierarchy Panel
        m_SceneHierarchyPanel.OnImGuiRender();

        // Viewport
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
        ImGui::Begin("Viewport");

        isViewportFocused = ImGui::IsWindowFocused();
        isViewportHovered = ImGui::IsWindowHovered();
        Application::Get().GetImGuiLayer()->SetBlocked(!isViewportFocused && !isViewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { viewportPanelSize.x,viewportPanelSize.y };

        uint32_t texID = m_FrameBuffer->GetColorAttachmentRendererID();
        ImGui::Image((ImTextureID)(uintptr_t)texID, ImVec2{ m_ViewportSize.x,m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));

        // Gizmo
        Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
        if (selectedEntity.IsValid() && m_GizmoType != -1)
        {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();

            float width = (float)ImGui::GetWindowWidth();
            float height = (float)ImGui::GetWindowHeight();
            ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, width, height);

            // Runtime Camera
            /*auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
            glm::mat4 camView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

            const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
            const glm::mat4& cameraProjection = camera.GetProjection();*/

            // Editor Camera
            glm::mat4 camView = m_EditorCamera.GetViewMatrix();
            const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();


            auto& tc = selectedEntity.GetComponent<TransformComponent>();
            glm::mat4 transform = tc.GetTransform();

            bool snap = Input::IsKeyPressed(EE_KEY_LEFT_CONTROL);
            float snapValue = 0.5f;
            if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
            {
                snapValue = 45.0f;
            }
            float snapValues[3] = { snapValue,snapValue,snapValue };

            ImGuizmo::Manipulate(glm::value_ptr(camView), glm::value_ptr(cameraProjection),
                m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);

            if (ImGuizmo::IsUsing())
            {
                glm::vec3 outTranslation, outScale, skew;
                glm::quat outRotation;
                glm::vec4 perspective;

                glm::decompose(transform, outScale, outRotation, outTranslation, skew, perspective);

                tc.Translation = outTranslation;
                tc.Rotation = glm::eulerAngles(outRotation);
                tc.Scale = outScale;
            }
        }

        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::End();
    }

    void EditorLayer::OnEvent(Event& event)
    {
        m_CameraController.OnEvent(event);
        m_EditorCamera.OnEvent(event);

        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(EE_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
    }

    bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
    {
        if (e.GetRepeatCount() > 0)
        {
            return false;
        }

        bool control = Input::IsKeyPressed(EE_KEY_LEFT_CONTROL) || Input::IsKeyPressed(EE_KEY_RIGHT_CONTROL);
        bool shift = Input::IsKeyPressed(EE_KEY_LEFT_SHIFT) || Input::IsKeyPressed(EE_KEY_RIGHT_SHIFT);
        switch (e.GetKeyCode())
        {
            case EE_KEY_S:
            {
                if (control && shift)
                {
                    SaveSceneAs();
                }
                break;
            }
            case EE_KEY_O:
            {
                if (control)
                {
                    OpenScene();
                }
                break;
            }
            case EE_KEY_N:
            {
                if (control)
                {
                    NewScene();
                }
                break;
            }

            // Gizmo
            case EE_KEY_Q:
            {
                m_GizmoType = ImGuizmo::OPERATION::UNIVERSAL;
                break;
            }
            case EE_KEY_W:
            {
                m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
                break;
            }
            case EE_KEY_E:
            {
                m_GizmoType = ImGuizmo::OPERATION::ROTATE;
                break;
            }
            case EE_KEY_R:
            {
                m_GizmoType = ImGuizmo::OPERATION::SCALE;
                break;
            }
        }
    }

    void EditorLayer::SaveSceneAs()
    {
        std::string filepath = FileDialogs::SaveFile("Ellie Scene (*.ellie)\0*.ellie\0");
        if (!filepath.empty())
        {
            SceneSerializer serializer(m_ActiveScene);
            serializer.Serialize(filepath);
        }
    }
    
    void EditorLayer::NewScene()
    {
        m_ActiveScene = std::make_shared<Scene>();
        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }
    
    void EditorLayer::OpenScene()
    {
        std::string filepath = FileDialogs::OpenFile("Ellie Scene (*.ellie)\0*.ellie\0");
        if (!filepath.empty())
        {
            m_ActiveScene = std::make_shared<Scene>();
            m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_SceneHierarchyPanel.SetContext(m_ActiveScene);

            SceneSerializer serializer(m_ActiveScene);
            serializer.Deserialize(filepath);
        }
    }
}