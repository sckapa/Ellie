#pragma once

namespace Ellie {

	class EditorLayer : public Ellie::Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnEvent(Ellie::Event& event) override;
		virtual void OnUpdate(Ellie::Timestep ts) override;

		virtual void OnImGuiRender() override;


	private:
		Ellie::OrthographicCameraController m_CameraController;

		Ellie::Ref<Ellie::Texture2D> m_Checker;

		Ellie::Ref<Ellie::FrameBuffer> m_FrameBuffer;

		glm::vec2 m_ViewportSize = { 0.0f,0.0f };
	};

}