#pragma once

class Sandbox2D : public Ellie::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D();

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	virtual void OnEvent(Ellie::Event& event) override;
	virtual void OnUpdate(Ellie::Timestep ts) override;

	virtual void OnImGuiRender() override;
	

private:
	Ellie::OrthographicCameraController m_CameraController;

	Ellie::Ref<Ellie::Texture2D> m_Checker;
	Ellie::Ref<Ellie::Texture2D> m_SpriteSheet;

	Ellie::Ref<Ellie::FrameBuffer> m_FrameBuffer;

	Ellie::Ref<Ellie::Shader> m_FlatColorShader;
	Ellie::Ref<Ellie::VertexArray> m_SqVertexArray;

	glm::vec4 m_Color = { 0.2f,0.3f,0.8f,1.0f };
};

