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

    extern const std::filesystem::path m_AssetPath;

    ImGuizmo::OPERATION m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;

    EditorLayer::EditorLayer() : Layer("Sandbox2D")
    {
    }

    EditorLayer::~EditorLayer()
    {
    }

    void EditorLayer::OnAttach()
    {
        m_PlayButton = Texture2D::Create("Resources/Icons/ToolbarIcons/PlayButton.png");
        m_StopButton = Texture2D::Create("Resources/Icons/ToolbarIcons/StopButton.png");

        FrameBufferSpecification spec;
        spec.Attachments = { FrameBufferTextureFormat::RGBA8,FrameBufferTextureFormat::RED_INTEGER, FrameBufferTextureFormat::Depth };
        spec.width = 1280;
        spec.height = 720;
        m_FrameBuffer = FrameBuffer::Create(spec);

        m_ActiveScene = std::make_shared<Scene>();
        m_EditorScene = std::make_shared<Scene>();

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
                if (Input::IsKeyPressed(A))
                {
                    tr[3][0] -= 5.0f * ts;
                }
                if (Input::IsKeyPressed(D))
                {
                    tr[3][0] += 5.0f * ts;
                }
                if (Input::IsKeyPressed(W))
                {
                    tr[3][1] += 5.0f * ts;
                }
                if (Input::IsKeyPressed(S))
                {
                    tr[3][1] -= 5.0f * ts;
                }
            }
        };

        m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

#endif
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
            m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
            m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
        }

        // Renderer
        Renderer2D::ResetStats();

        m_FrameBuffer->Bind();

        RenderCommands::SetClearColor({ 0.1f,0.1f,0.1f,1.0f });
        RenderCommands::Clear();

        // Clear our entity ID attachment to -1
        m_FrameBuffer->ClearAttachment(1, -1);

        if (m_SceneState == SceneState::Edit)
        {
            if (isViewportFocused)
            {
                m_EditorCamera.OnUpdate(ts);
            }

            m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);
        }
        else if (m_SceneState == SceneState::Play)
        {
            m_ActiveScene->OnUpdateRuntime(ts);
        }

        auto [mx, my] = ImGui::GetMousePos();
        mx -= m_ViewportBounds[0].x;
        my -= m_ViewportBounds[0].y;

        glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
        my = viewportSize.y - my;

        int mouseX = (int)mx;
        int mouseY = (int)my;

        if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
        {
            int pixelData = m_FrameBuffer->ReadPixel(1, mouseX, mouseY);
            m_SelectedEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_ActiveScene.get());
        }

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
        m_ContentBrowserPanel.OnImguiRender();

        // Viewport
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
        ImGui::Begin("Viewport");

        auto viewposrtMinRegion = ImGui::GetWindowContentRegionMin();
        auto viewposrtMaxRegion = ImGui::GetWindowContentRegionMax();
        auto viewportOffset = ImGui::GetWindowPos();
        m_ViewportBounds[0] = { viewposrtMinRegion.x + viewportOffset.x,viewposrtMinRegion.y + viewportOffset.y };
        m_ViewportBounds[1] = { viewposrtMaxRegion.x + viewportOffset.x,viewposrtMaxRegion.y + viewportOffset.y };

        isViewportFocused = ImGui::IsWindowFocused();
        isViewportHovered = ImGui::IsWindowHovered();
        Application::Get().GetImGuiLayer()->SetBlocked(!isViewportFocused && !isViewportHovered);

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_ViewportSize = { viewportPanelSize.x,viewportPanelSize.y };

        uint32_t texID = m_FrameBuffer->GetColorAttachmentRendererID();
        ImGui::Image((ImTextureID)(uintptr_t)texID, ImVec2{ m_ViewportSize.x,m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
            {
                const wchar_t* path = (const wchar_t*)payload->Data;
                OpenScene(std::filesystem::path(m_AssetPath) / path);
            }
            ImGui::EndDragDropTarget();
        }

        // Gizmo
        Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
        if (selectedEntity.IsValid() && m_SceneState == SceneState::Edit)
        {
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetDrawlist();

            ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

            // Editor Camera
            glm::mat4 camView = m_EditorCamera.GetViewMatrix();
            const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();


            auto& tc = selectedEntity.GetComponent<TransformComponent>();
            glm::mat4 transform = tc.GetTransform();

            bool snap = Input::IsKeyPressed(Ellie::Key::LEFT_CONTROL);
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

        UI_Toolbar();

        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::End();
    }

    void EditorLayer::DuplicateEntity()
    {
        if (m_SceneState != SceneState::Edit)
        {
            return;
        }

        Entity selected = m_SceneHierarchyPanel.GetSelectedEntity();
        if (selected.IsValid())
        {
            m_EditorScene->DuplicateEntity(selected);
        }
    }

    void EditorLayer::UI_Toolbar()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

        ImGui::Begin("##Toolbar", nullptr, ImGuiViewportFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        float size = ImGui::GetWindowHeight() - 4.0f;
        Ref<Texture2D> m_Icon = (m_SceneState == SceneState::Edit) ? m_PlayButton : m_StopButton;
        ImGui::SetCursorPosX((ImGui::GetContentRegionMax().x * 0.5f) - (size * 0.5f));
        if (ImGui::ImageButton("##playpause", m_Icon->GetRendererID(), ImVec2(size, size)))
        {
            if (m_SceneState == SceneState::Edit)
            {
                OnScenePlay();
            }
            else if (m_SceneState == SceneState::Play)
            {
                OnSceneStop();
            }
        }

        ImGui::PopStyleVar(2);
        ImGui::PopStyleColor();

        ImGui::End();
    }

    void EditorLayer::OnEvent(Event& event)
    {
        m_EditorCamera.OnEvent(event);

        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<KeyPressedEvent>(EE_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
        dispatcher.Dispatch<MouseButtonPressedEvent>(EE_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
    }

    bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
    {
        if (e.GetRepeatCount() > 0)
        {
            return false;
        }

        bool control = Input::IsKeyPressed(Ellie::Key::LEFT_CONTROL) || Input::IsKeyPressed(Ellie::Key::RIGHT_CONTROL);
        bool shift = Input::IsKeyPressed(Ellie::Key::LEFT_SHIFT) || Input::IsKeyPressed(Ellie::Key::RIGHT_SHIFT);
        switch (e.GetKeyCode())
        {
            case Ellie::Key::S:
            {
                if (control && shift)
                {
                    SaveSceneAs();
                }
                else if (control)
                {
                    SaveScene();
                }
                break;
            }
            case Ellie::Key::O:
            {
                if (control)
                {
                    OpenScene();
                }
                break;
            }
            case Ellie::Key::N:
            {
                if (control)
                {
                    NewScene();
                }
                break;
            }

            // Scene Controls
            case Ellie::Key::D:
            {
                if (control)
                {
                    DuplicateEntity();
                }
                break;
            }

            // Gizmo
            case Ellie::Key::Q:
            {
                m_GizmoType = ImGuizmo::OPERATION::UNIVERSAL;
                break;
            }
            case Ellie::Key::W:
            {
                m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
                break;
            }
            case Ellie::Key::E:
            {
                m_GizmoType = ImGuizmo::OPERATION::ROTATE;
                break;
            }
            case Ellie::Key::R:
            {
                m_GizmoType = ImGuizmo::OPERATION::SCALE;
                break;
            }
        }
    }

    bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
    {
        if ((e.GetMouseButton() == Ellie::Mouse::MOUSE_LEFT) 
            && isViewportHovered 
            && !ImGuizmo::IsOver() 
            && !Input::IsKeyPressed(Ellie::Key::LEFT_ALT))
        {
            m_SceneHierarchyPanel.SetSelectedEntity(m_SelectedEntity);
        }
        return false;
    }

    void EditorLayer::SaveScene()
    {
        if (!m_EditorPath.empty())
        {
            SceneSerializer serializer(m_ActiveScene);
            serializer.Serialize(m_EditorPath.string());
        }
        else
        {
            SaveSceneAs();
        }
    }

    void EditorLayer::SaveSceneAs()
    {
        std::string filepath = FileDialogs::SaveFile("Ellie Scene (*.ellie)\0*.ellie\0");
        if (!filepath.empty())
        {
            SceneSerializer serializer(m_ActiveScene);
            serializer.Serialize(filepath);

            m_EditorPath = filepath;
        }
    }
    
    void EditorLayer::NewScene()
    {
        m_ActiveScene = std::make_shared<Scene>();
        m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
        m_SceneHierarchyPanel.SetContext(m_ActiveScene);

        m_EditorPath = std::filesystem::path();
    }
    
    void EditorLayer::OpenScene()
    {
        std::string filepath = FileDialogs::OpenFile("Ellie Scene (*.ellie)\0*.ellie\0");
        if (!filepath.empty())
        {
            OpenScene(filepath);
        }
    }

    void EditorLayer::OpenScene(std::filesystem::path path)
    {
        if (m_SceneState != SceneState::Edit)
        {
            OnSceneStop();
        }

        Ref<Scene> newScene = std::make_shared<Scene>();
        SceneSerializer serializer(newScene);
        if (serializer.Deserialize(path.string()))
        {
            m_EditorScene = newScene;
            m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
            m_SceneHierarchyPanel.SetContext(m_EditorScene);  

            m_ActiveScene = m_EditorScene;

            m_EditorPath = path;
        }
    }

    void EditorLayer::OnScenePlay()
    {
        m_SceneState = SceneState::Play;
        m_ActiveScene = Scene::Copy(m_EditorScene);
        m_ActiveScene->OnRuntimeStart();

        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }

    void EditorLayer::OnSceneStop()
    {
        m_SceneState = SceneState::Edit;
        m_ActiveScene->OnRuntimeStop();
        m_ActiveScene = m_EditorScene;

        m_SceneHierarchyPanel.SetContext(m_ActiveScene);
    }
}