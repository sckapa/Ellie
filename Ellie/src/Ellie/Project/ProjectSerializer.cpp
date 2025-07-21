#include "eepch.h"
#include "ProjectSerializer.h"

#include "yaml-cpp/yaml.h"
#include <fstream>


namespace Ellie {

	ProjectSerializer::ProjectSerializer(Ref<Project> project) : m_Project(project)
	{
	}

	bool ProjectSerializer::Serialize(const std::filesystem::path& path)
	{
		const auto& projectConfig = m_Project->GetConfig();

		YAML::Emitter out;

		out << YAML::BeginMap; // Root
		{
			out << YAML::Key << "Project" << YAML::Value;
			{
				out << YAML::BeginMap; // Project
				out << YAML::Key << "Name" << YAML::Value << projectConfig.name;
				out << YAML::Key << "StartScene" << YAML::Value << projectConfig.StartScene.string();
				out << YAML::Key << "AssetDirectory" << YAML::Value << projectConfig.AssetDirectory.string();
				out << YAML::Key << "ScriptModulePath" << YAML::Value << projectConfig.ScriptModuelPath.string();
			}
			out << YAML::EndMap; // Project
		}
		out << YAML::EndMap; // Root

		std::ofstream fout(path);
		fout << out.c_str();

		return true;
	}

	bool ProjectSerializer::Deserialize(const std::filesystem::path& path)
	{
		auto& projectConfig = m_Project->GetConfig();

		YAML::Node data;
		try
		{
			data = YAML::LoadFile(path.string());
		}
		catch (YAML::ParserException e)
		{
			EE_CORE_ERROR("Failed to load project file '{0}'\n		{1}", path.string(), e.what());
			return false;
		}

		auto projectNode = data["Project"];
		if (!projectNode)
		{
			return false;
		}

		projectConfig.name = projectNode["Name"].as<std::string>();
		projectConfig.StartScene = projectNode["StartScene"].as<std::string>();
		projectConfig.AssetDirectory = projectNode["AssetDirectory"].as<std::string>();
		projectConfig.ScriptModuelPath = projectNode["ScriptModulePath"].as<std::string>();

		return true;
	}

}