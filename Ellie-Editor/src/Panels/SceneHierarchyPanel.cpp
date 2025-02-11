#include "SceneHierarchyPanel.h"
#include <imgui.h>
#include "Ellie/Scene/Components.h"

namespace Ellie {

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene> context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene> context)
	{
		m_Context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		for (auto entity : m_Context->m_Registry.view<TagComponent>())
		{
			auto& tc = m_Context->m_Registry.get<TagComponent>(entity);
			ImGui::Text("%s", tc.Tag.c_str());
		}

		ImGui::End();
	}

}
