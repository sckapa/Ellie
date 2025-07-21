#pragma once

#include "Ellie/Core/Core.h"
#include "Project.h"

namespace Ellie {

	class ProjectSerializer
	{
	public:
		ProjectSerializer(Ref<Project> project);

		bool Serialize(const std::filesystem::path& path);
		bool Deserialize(const std::filesystem::path& path);

	private:
		Ref<Project> m_Project;
	};

}