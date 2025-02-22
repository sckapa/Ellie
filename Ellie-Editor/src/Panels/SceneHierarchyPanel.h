#pragma once

#include "Ellie/Core/Core.h"
#include "Ellie/Core/Log.h"
#include "Ellie/Scene/Scene.h"
#include "Ellie/Scene/Entity.h"

namespace Ellie {

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene> context);

		void SetContext(const Ref<Scene> context);
		void OnImGuiRender();

		Entity GetSelectedEntity() { return m_SelectedContext; }
		void SetSelectedEntity(Entity entity) { m_SelectedContext = entity; }

	private:
		void DrawEntityNode(Entity entity);
		void DrawInspector(Entity entity);

		Entity m_SelectedContext;
		Ref<Scene> m_Context;
	};

}