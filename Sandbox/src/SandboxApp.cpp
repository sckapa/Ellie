#include <Ellie.h>

class Sandbox : public Ellie::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

Ellie::Application* Ellie::CreateApplication()
{
	return new Sandbox();
}