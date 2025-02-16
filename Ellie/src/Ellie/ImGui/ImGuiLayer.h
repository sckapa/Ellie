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
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void SetBlocked(bool blocked) { m_isBlocked = blocked; }

		void SetDarkThemeColors();

	private:
		bool m_isBlocked = true;
	};

}

