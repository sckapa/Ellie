#pragma once

#include "Ellie/Core/Core.h"
#include <filesystem>

namespace Ellie {

	struct ProjectConfig
	{
		std::string name = "Untitled";

		std::filesystem::path StartScene;

		std::filesystem::path AssetDirectory;
		std::filesystem::path ScriptModuelPath;
	};

	class Project
	{
	public:

		ProjectConfig& GetConfig() { return m_ProjectConfig; }
		static Ref<Project> Get() { return s_ActiveProject; }

		static const std::filesystem::path& GetProjectDirectory();
		static std::filesystem::path GetAssetDirectory();
		// Move to asset manager later
		static std::filesystem::path GetAssetFileSystemPath(const std::filesystem::path& path);

		static Ref<Project> New();
		static Ref<Project> Load(const std::filesystem::path& path);
		static bool SaveActive(const std::filesystem::path& path);

	private:
		inline static Ref<Project> s_ActiveProject;
		ProjectConfig m_ProjectConfig;
		std::filesystem::path m_ProjectDirectory;
	};

}
