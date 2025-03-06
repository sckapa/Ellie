#pragma once

#include "Ellie.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Ellie/Events/KeyEvent.h"
#include "Ellie/Renderer/EditorCamera.h"

namespace Ellie {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnEvent(Event& event) override;
		virtual void OnUpdate(Timestep ts) override;

		virtual void OnImGuiRender() override;

	private:
		// Events
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		// File system
		std::filesystem::path m_EditorPath;
		void SaveScene();
		void SaveSceneAs();
		void NewScene();
		void OpenScene();
		void OpenScene(std::filesystem::path path);

		// Scene Controls
		void DuplicateEntity();

		void UI_Toolbar();
		void OnScenePlay();
		void OnSceneStop();

		bool isViewportFocused = false, isViewportHovered = false;

		EditorCamera m_EditorCamera;

		Ref<Texture2D> m_PlayButton;
		Ref<Texture2D> m_StopButton;

		Ref<FrameBuffer> m_FrameBuffer;

		glm::vec2 m_ViewportSize = { 0.0f,0.0f };
		glm::vec2 m_ViewportBounds[2];
		Entity m_SelectedEntity = Entity();

		Ref<Scene> m_ActiveScene;
		Ref<Scene> m_EditorScene;

		SceneHierarchyPanel m_SceneHierarchyPanel;
		ContentBrowserPanel m_ContentBrowserPanel;

		enum class SceneState
		{
			Edit = 0, Play = 1
		};
		SceneState m_SceneState = SceneState::Edit;
	};

}