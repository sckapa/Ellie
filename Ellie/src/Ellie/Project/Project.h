#pragma once

#include "Ellie.h"
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
		static std::filesystem::path GetAssetDirectory()
		{
			EE_ASSERT(s_ActiveProject, "Project does not exist!");
			return GetProjectDirectory() / s_ActiveProject->m_ProjectConfig.AssetDirectory;
		}

		static const std::filesystem::path& GetProjectDirectory()
		{
			EE_ASSERT(s_ActiveProject, "Project does not exist!");
			return s_ActiveProject->m_ProjectDirectory;
		}
		
		// Move to asset manager later
		static std::filesystem::path GetAssetFileSystemPath(const std::filesystem::path& path)
		{
			EE_ASSERT(s_ActiveProject, "Project does not exist!");
			return GetAssetDirectory() / path;
		}

		ProjectConfig& GetConfig() { return m_ProjectConfig; }
		static Ref<Project> Get() { return s_ActiveProject; }

		static Ref<Project> New();
		static Ref<Project> Load(const std::filesystem::path& path);
		static bool SaveActive(const std::filesystem::path& path);

	private:
		inline static Ref<Project> s_ActiveProject;
		ProjectConfig m_ProjectConfig;
		std::filesystem::path m_ProjectDirectory;
	};

}
