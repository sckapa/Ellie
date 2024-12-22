#pragma once

#include "Core.h"
#include "Events/Event.h"

namespace Ellie{

	class ELLIE_API Application
	{
	public :
		Application();
		virtual ~Application();

		void Run();
	};

	// To be defined in a CLIENT
	Application* CreateApplication();
}

