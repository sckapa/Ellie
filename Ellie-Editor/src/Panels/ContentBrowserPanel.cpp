#include "Ellie/Project/Project.h"
#include "ContentBrowserPanel.h"
#include "imgui.h"

namespace Ellie {

	ContentBrowserPanel::ContentBrowserPanel() : m_BasePath(Project::GetAssetDirectory()), m_CurrentPath(m_BasePath)
	{
		m_FileIcon = Texture2D::Create("Resources/Icons/ContentBrowserIcons/DirectoryIcon.png");
		m_DirectoryIcon = Texture2D::Create("Resources/Icons/ContentBrowserIcons/FileIcon.png");
	}

	void ContentBrowserPanel::OnImguiRender()
	{
		ImGui::Begin("Content Browser");

		if (m_CurrentPath != std::filesystem::path(m_BasePath))
		{
			if (ImGui::Button("<-"))
			{
				m_CurrentPath = m_CurrentPath.parent_path();
			}
		}

		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = padding + thumbnailSize;

		auto panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);

		ImGui::Columns(std::max(1, columnCount), 0, false);

		Ref<Texture2D> m_Icon;

		for (auto& path : std::filesystem::directory_iterator(m_CurrentPath))
		{
			std::filesystem::path relativePath(path.path());
			std::string filenameString = relativePath.filename().string();

			m_Icon = path.is_directory() ? m_FileIcon : m_DirectoryIcon;

			ImGui::PushID(filenameString.c_str());

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			ImGui::ImageButton("##FileName", (ImTextureID)m_Icon->GetRendererID(), { thumbnailSize,thumbnailSize }, { 0,1 }, { 1,0 });
			
			if (ImGui::BeginDragDropSource())
			{
				const wchar_t* payload = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", payload, (wcslen(payload) + 1) * sizeof(wchar_t), ImGuiCond_Once);
				ImGui::EndDragDropSource();
			}
			
			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (path.is_directory())
				{
					m_CurrentPath /= path.path().filename();
				}
			}

			ImGui::PopID();

			ImGui::TextWrapped(filenameString.c_str());

			ImGui::NextColumn();
		}

		ImGui::Columns(1);

		ImGui::End();
	}

}