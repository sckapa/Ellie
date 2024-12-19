#pragma once

#include "Core.h"

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

