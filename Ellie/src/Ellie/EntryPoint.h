#pragma once

#ifdef EE_PLATFORM_WINDOWS

extern Ellie::Application* Ellie::CreateApplication();

int main(int argc, char** argv)
{
	Ellie::Log::Init();
	EE_CORE_INFO("Hi I'm yellow");
	int a = 5;
	EE_WARN("Hello {0}", a);

	auto app = Ellie::CreateApplication();
	app->Run();
	delete app;
}

#endif