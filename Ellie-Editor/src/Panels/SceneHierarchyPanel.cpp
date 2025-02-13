#include "SceneHierarchyPanel.h"
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
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

		for (auto entityID : m_Context->m_Registry.view<TagComponent>())
		{
			Entity entity{ entityID,m_Context.get() };
			DrawEntityNode(entity);
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_SelectedContext = {};
		}

		ImGui::End();

		ImGui::Begin("Inspector");

		if (m_SelectedContext.IsValid())
		{
			DrawInspector(m_SelectedContext);
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tc = entity.GetComponent<TagComponent>();
		ImGuiTreeNodeFlags flags = ((m_SelectedContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)entity, flags, tc.Tag.c_str());

		if (ImGui::IsItemClicked())
		{
			m_SelectedContext = entity;
		}

		if (opened)
		{
			ImGui::TreePop();
		}
	}

	void SceneHierarchyPanel::DrawInspector(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		if (entity.HasComponent<TransformComponent>())
		{
			if(ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transform = entity.GetComponent<TransformComponent>().Transform;
				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);
				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<CameraComponent>())
		{
			if(ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				auto& camera = entity.GetComponent<CameraComponent>().Camera;

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];

				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = projectionTypeStrings[i] == currentProjectionTypeString;
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}

					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					auto fov = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (ImGui::DragFloat("Vertical FOV", &fov))
					{
						camera.SetPerspectiveVerticalFOV(glm::radians(fov));
					}

					auto nearClip = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near", &nearClip))
					{
						camera.SetPerspectiveNearClip(nearClip);
					}

					auto farClip = camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far", &farClip))
					{
						camera.SetPerspectiveFarClip(farClip);
					}
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					auto size = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &size))
					{
						camera.SetOrthographicSize(size);
					}

					auto nearClip = camera.GetOrthoNearClip();
					if (ImGui::DragFloat("Near", &nearClip))
					{
						camera.SetOrthoNearClip(nearClip);
					}

					auto farClip = camera.GetOrthoFarClip();
					if (ImGui::DragFloat("Far", &farClip))
					{
						camera.SetOrthoFarClip(farClip);
					}
				}

				ImGui::TreePop();
			}
		}
	}

}
