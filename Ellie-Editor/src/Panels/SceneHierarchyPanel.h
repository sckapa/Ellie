#pragma once

#include "Ellie/Scene/Scene.h"
#include "Ellie/Core/Core.h"
#include "Ellie/Scene/Entity.h"
#include <Ellie.h>

namespace Ellie {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene> context);

		void SetContext(const Ref<Scene> context);
		void OnImGuiRender();

	private:
		Ref<Scene> m_Context;
	};

}