#include "eepch.h"
#include "Project.h"

#include "ProjectSerializer.h"

namespace Ellie {

	const std::filesystem::path& Project::GetProjectDirectory()
	{
		EE_CORE_ASSERT(s_ActiveProject, "Project does not exist!");
		return s_ActiveProject->m_ProjectDirectory;
	}

	std::filesystem::path Project::GetAssetDirectory()
	{
		EE_CORE_ASSERT(s_ActiveProject, "Project does not exist!");
		return GetProjectDirectory() / s_ActiveProject->m_ProjectConfig.AssetDirectory;
	}

	std::filesystem::path Project::GetAssetFileSystemPath(const std::filesystem::path& path)
	{
		EE_CORE_ASSERT(s_ActiveProject, "Project does not exist!");
		return GetAssetDirectory() / path;
	}

	Ref<Project> Project::New()
	{
		s_ActiveProject = std::make_shared<Project>();
		return s_ActiveProject;
	}

	Ref<Project> Project::Load(const std::filesystem::path& path)
	{
		Ref<Project> project = std::make_shared<Project>();
		ProjectSerializer serializer(project);

		if (serializer.Deserialize(path))
		{
			project->m_ProjectDirectory = path.parent_path();
			s_ActiveProject = project;
			return s_ActiveProject;
		}

		return nullptr;
	}

	bool Project::SaveActive(const std::filesystem::path& path)
	{
		ProjectSerializer serializer(s_ActiveProject);
		if (serializer.Serialize(path))
		{
			s_ActiveProject->m_ProjectDirectory = path.parent_path();
			return true;
		}

		return false;
	}

}