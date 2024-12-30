#pragma once

#include "Core.h"
#include "Ellie/Events/Event.h"


namespace Ellie{

	class ELLIE_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnEvent(Event& event) {}
		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}


		virtual void OnImGuiRender() {}

		inline const std::string& GetLayerName() const { return m_DebugName; }

	private:
		std::string m_DebugName;
	};
}
