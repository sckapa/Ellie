#pragma once

#ifdef EE_PLATFORM_WINDOWS

extern Ellie::Application* Ellie::CreateApplication();

int main(int argc, char** argv)
{
	Ellie::Log::Init();

	auto app = Ellie::CreateApplication();
	app->Run();
	delete app;
}

#endif