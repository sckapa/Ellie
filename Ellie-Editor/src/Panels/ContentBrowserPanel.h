#pragma once

#include <filesystem>
#include "Ellie/Renderer/Texture.h"

namespace Ellie {

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnImguiRender();

	private:
		std::filesystem::path m_CurrentPath;

		Ref<Texture2D> m_DirectoryIcon;
		Ref<Texture2D> m_FileIcon;
	};

}
