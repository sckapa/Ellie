#pragma once

#include "Ellie/Core/Core.h"
#include "Ellie/Core/Layer.h"

namespace Ellie{

	class ELLIE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	};

}

