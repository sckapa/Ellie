#pragma once

#include "Ellie.h"
#include "Panels/SceneHierarchyPanel.h"

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
		bool isViewportFocused = false, isViewportHovered = false;

		OrthographicCameraController m_CameraController;

		Ref<Texture2D> m_Checker;

		Ref<FrameBuffer> m_FrameBuffer;

		glm::vec2 m_ViewportSize = { 0.0f,0.0f };

		// Delete
		Entity m_SquareEntity;
		Entity m_CameraEntity;
		//

		Ref<Scene> m_ActiveScene;

		SceneHierarchyPanel m_SceneHierarchyPanel;
	};

}