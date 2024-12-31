#include <Ellie.h>

class ExampleLayer : public Ellie::Layer
{
public:
	ExampleLayer() : Layer("Example") { }

	void OnEvent(Ellie::Event& e) override
	{
		//EE_INFO(e);
	}

	void OnUpdate() override
	{
		//EE_INFO("Update called");
	}
};

class Sandbox : public Ellie::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

Ellie::Application* Ellie::CreateApplication()
{
	return new Sandbox();
}