#include "eepch.h"
#include "SceneSerializer.h"
#include <yaml-cpp/yaml.h>
#include "Ellie/Scene/Components.h"
#include "Ellie/Core/UUID.h"
#include "Ellie/Scripting/ScriptEngine.h"
#include <fstream>
#include "ScriptableEntity.h"

namespace YAML {

	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
			{
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
 		}
	};
	
	template<>
	struct convert<Ellie::UUID>
	{
		static Node encode(const Ellie::UUID& uuid)
		{
			Node node;
			node.push_back(uuid);
			return node;
		}

		static bool decode(const Node& node, Ellie::UUID& uuid)
		{
			if (!node.IsSequence() || node.size() != 2)
			{
				return false;
			}

			uuid = node[0].as<float>();
			return true;
 		}
	};
	
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
			{
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
 		}
	};
	
	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
			{
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
 		}
	};

}

namespace Ellie {

#define WRITE_FIELD_DATA(FieldType, Type) case ScriptFieldType::FieldType:\
											out << YAML::Value << scriptFieldInstance.GetValue<Type>();\
											break

	namespace Utils {

		static const char* ScriptFieldTypeToString(Ellie::ScriptFieldType type)
		{
			switch (type)
			{
			case Ellie::ScriptFieldType::Float:   return "Float";
			case Ellie::ScriptFieldType::Double:  return "Double";
			case Ellie::ScriptFieldType::Char:    return "Char";
			case Ellie::ScriptFieldType::Bool:    return "Boolean";
			case Ellie::ScriptFieldType::Byte:    return "Byte";
			case Ellie::ScriptFieldType::Short:   return "Short";
			case Ellie::ScriptFieldType::Int:     return "Integer";
			case Ellie::ScriptFieldType::Long:    return "Long";
			case Ellie::ScriptFieldType::UShort:  return "UShort";
			case Ellie::ScriptFieldType::UInt:    return "UInteger";
			case Ellie::ScriptFieldType::ULong:   return "ULong";
			case Ellie::ScriptFieldType::Vector2: return "Vector2";
			case Ellie::ScriptFieldType::Vector3: return "Vector3";
			case Ellie::ScriptFieldType::Vector4: return "Vector4";
			case Ellie::ScriptFieldType::Entity:  return "Entity";
			}

			return "<Invalid>";
		}

		static Ellie::ScriptFieldType ScriptFieldTypeFromString(std::string_view type)
		{
			if (type == "Float")    return Ellie::ScriptFieldType::Float;
			if (type == "Double")   return Ellie::ScriptFieldType::Double;
			if (type == "Char")     return Ellie::ScriptFieldType::Char;
			if (type == "Bool")     return Ellie::ScriptFieldType::Bool;
			if (type == "Byte")     return Ellie::ScriptFieldType::Byte;
			if (type == "Short")    return Ellie::ScriptFieldType::Short;
			if (type == "Int")      return Ellie::ScriptFieldType::Int;
			if (type == "Long")     return Ellie::ScriptFieldType::Long;
			if (type == "UShort")   return Ellie::ScriptFieldType::UShort;
			if (type == "UInt")     return Ellie::ScriptFieldType::UInt;
			if (type == "ULong")    return Ellie::ScriptFieldType::ULong;
			if (type == "Vector2")  return Ellie::ScriptFieldType::Vector2;
			if (type == "Vector3")  return Ellie::ScriptFieldType::Vector3;
			if (type == "Vector4")  return Ellie::ScriptFieldType::Vector4;
			if (type == "Entity")   return Ellie::ScriptFieldType::Entity;

			EE_ASSERT(false, "Unknown type!")
				return Ellie::ScriptFieldType::None;
		}

	}

	YAML::Emitter& operator <<(YAML::Emitter& out, const glm::vec2 v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}
	
	YAML::Emitter& operator <<(YAML::Emitter& out, const glm::vec3 v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator <<(YAML::Emitter& out, const glm::vec4 v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene) : m_Scene(scene)
	{
	}

	static std::string RigidbodyTypeToString(Rigidbody2DComponent::Bodytype& type)
	{
		switch (type)
		{
		case Rigidbody2DComponent::Bodytype::Dynamic: return "Dynamic";
		case Rigidbody2DComponent::Bodytype::Static: return "Static";
		case Rigidbody2DComponent::Bodytype::Kinematic: return "Kinematic";
		}
	}

	static Rigidbody2DComponent::Bodytype RigidbodyTypeFromString(std::string& type)
	{
		if (type == "Static") return Rigidbody2DComponent::Bodytype::Static;
		if (type == "Dynamic") return Rigidbody2DComponent::Bodytype::Dynamic;
		if (type == "Kinematic") return Rigidbody2DComponent::Bodytype::Kinematic;
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity";
		out << YAML::Value << entity.GetUUID();

		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::BeginMap;
			out << YAML::Key << "Tag" << YAML::Value << tag;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::BeginMap;
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;

			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = entity.GetComponent<CameraComponent>().Camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap;
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::Key << "OrthoGraphicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthoNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthoFarClip();
			out << YAML::EndMap;
				
			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;
			out << YAML::EndMap;
		}
		
		if (entity.HasComponent<ScriptComponent>())
		{
			out << YAML::Key << "ScriptComponent";
			out << YAML::BeginMap;

			auto& scriptComponent = entity.GetComponent<ScriptComponent>();

			out << YAML::Key << "Class Name" << YAML::Value << scriptComponent.ClassName;

			Ref<ScriptClass> scriptClass = ScriptEngine::GetEntityClass(scriptComponent.ClassName);
			const auto& fields = scriptClass->GetPublicFields();

			if (fields.size() > 0)
			{
				out << YAML::Key << "ScriptFields" << YAML::Value;
				out << YAML::BeginSeq;
				ScriptFieldMap& scriptFieldMap = ScriptEngine::GetScriptFieldMap(entity);
				for (auto& [name, field] : fields)
				{
					if (scriptFieldMap.find(name) == scriptFieldMap.end())
					{
						continue;
					}

					out << YAML::BeginMap;
					out << YAML::Key << "Name" << YAML::Value << name;
					out << YAML::Key << "Type" << YAML::Value << Utils::ScriptFieldTypeToString(field.type);
					out << YAML::Key << "Data" << YAML::Value;

					ScriptFieldInstance& scriptFieldInstance = scriptFieldMap.at(name);
					
					switch (field.type)
					{
						WRITE_FIELD_DATA(Float, float);
						WRITE_FIELD_DATA(Double, double);
						WRITE_FIELD_DATA(Bool, bool);
						WRITE_FIELD_DATA(Byte, int8_t);
						WRITE_FIELD_DATA(Short, int16_t);
						WRITE_FIELD_DATA(Int, int32_t);
						WRITE_FIELD_DATA(Long, int64_t);
						WRITE_FIELD_DATA(Char, uint8_t);
						WRITE_FIELD_DATA(UShort, uint16_t);
						WRITE_FIELD_DATA(UInt, uint32_t);
						WRITE_FIELD_DATA(ULong, uint64_t);
						WRITE_FIELD_DATA(Vector2, glm::vec2);
						WRITE_FIELD_DATA(Vector3, glm::vec3);
						WRITE_FIELD_DATA(Vector4, glm::vec4);
						WRITE_FIELD_DATA(Entity, UUID);
					}
					out << YAML::EndMap;
				}
				out << YAML::EndSeq;
			}

			out << YAML::EndMap;
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			auto& sc = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::BeginMap;
			out << YAML::Key << "Color" << YAML::Value << sc.Color;
			out << YAML::EndMap;
		}

		if (entity.HasComponent<Rigidbody2DComponent>())
		{
			out << YAML::Key << "Rigidbody2DComponent";
			auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
			out << YAML::BeginMap;
			out << YAML::Key << "Type" << YAML::Value << RigidbodyTypeToString(rb2d.Type);
			out << YAML::Key << "FixedRotation" << YAML::Value << rb2d.FixedRotation;
			out << YAML::EndMap;
		}
		

		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			out << YAML::Key << "BoxCollider2DComponent";
			auto& bc2d = entity.GetComponent<BoxCollider2DComponent>();
			out << YAML::BeginMap;
			out << YAML::Key << "Offset" << YAML::Value << bc2d.Offset;
			out << YAML::Key << "Size" << YAML::Value << bc2d.Size;
			out << YAML::Key << "Density" << YAML::Value << bc2d.Density;
			out << YAML::Key << "Friction" << YAML::Value << bc2d.Friction;
			out << YAML::Key << "Restitution" << YAML::Value << bc2d.Restitution;
			out << YAML::Key << "RestitutionThreshold" << YAML::Value << bc2d.RestitutionThreshold;
			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		for (const auto [ent, ref] : m_Scene->m_Registry.storage<TagComponent>().each())
		{
			Entity entity{ ent,m_Scene.get() };
			SerializeEntity(out, entity);
		}

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
		EE_ASSERT(false, "Not implemented yet");
	}

	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strstream;
		strstream << stream.rdbuf();

		YAML::Node data = YAML::Load(strstream.str());

		if (!data["Scene"])
		{
			return false;
		}

		std::string sceneName = data["Scene"].as<std::string>();

		auto entities = data["Entities"];
		for (auto entity : entities)
		{
			uint64_t uuid = entity["Entity"].as<uint64_t>();

			std::string name;
			auto tc = entity["TagComponent"];
			if (tc)
			{
				name = tc["Tag"].as<std::string>();
			}
			
			Entity deserializedEntity = m_Scene->CreateEntityWithUUID(uuid, name);

			auto transformComponent = entity["TransformComponent"];
			if (transformComponent)
			{
				auto& tc = deserializedEntity.GetComponent<TransformComponent>();
				tc.Translation = transformComponent["Translation"].as<glm::vec3>();
				tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
				tc.Scale = transformComponent["Scale"].as<glm::vec3>();
			}

			auto cameraComponent = entity["CameraComponent"];
			if (cameraComponent)
			{
				auto& cc = deserializedEntity.AddComponent<CameraComponent>();
				auto& cameraProps = cameraComponent["Camera"];

				cc.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

				cc.Camera.SetPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
				cc.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
				cc.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

				cc.Camera.SetOrthographicSize(cameraProps["OrthoGraphicSize"].as<float>());
				cc.Camera.SetOrthoNearClip(cameraProps["OrthographicNear"].as<float>());
				cc.Camera.SetOrthoFarClip(cameraProps["OrthographicFar"].as<float>());

				cc.Primary = cameraComponent["Primary"].as<bool>();
				cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
			}

			auto spriteComponent = entity["SpriteRendererComponent"];
			if (spriteComponent)
			{
				auto& sc = deserializedEntity.AddComponent<SpriteRendererComponent>();
				sc.Color = spriteComponent["Color"].as<glm::vec4>();
			}
			
			auto scriptComponent = entity["ScriptComponent"];
			if (scriptComponent)
			{
				auto& sc = deserializedEntity.AddComponent<ScriptComponent>();
				sc.ClassName = scriptComponent["Class Name"].as<std::string>();
			}

			auto rb2d = entity["Rigidbody2DComponent"];
			if (rb2d)
			{
				auto& rb = deserializedEntity.AddComponent<Rigidbody2DComponent>();
				rb.FixedRotation = rb2d["FixedRotation"].as<bool>();
				rb.Type = RigidbodyTypeFromString(rb2d["Type"].as<std::string>());
			}

			auto bc2d = entity["BoxCollider2DComponent"];
			if (bc2d)
			{
				auto& bc = deserializedEntity.AddComponent<BoxCollider2DComponent>();
				bc.Density = bc2d["Density"].as<float>();
				bc.Friction = bc2d["Friction"].as<float>();
				bc.Restitution = bc2d["Restitution"].as<float>();
				bc.RestitutionThreshold = bc2d["RestitutionThreshold"].as<float>();
				bc.Offset = bc2d["Offset"].as<glm::vec2>();
				bc.Size = bc2d["Size"].as<glm::vec2>();
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath)
	{
		EE_ASSERT(false, "Not implemented yet");
		return false;
	}

}