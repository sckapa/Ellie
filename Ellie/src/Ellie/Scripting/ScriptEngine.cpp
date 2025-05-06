#include "eepch.h"
#include "ScriptEngine.h"
#include "glm/glm.hpp"

#include "Ellie/Core/UUID.h"

#include "ScriptGlue.h"
#include "mono/metadata/object.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/appdomain.h>
#include <mono/metadata/tabledefs.h>

namespace Ellie {

	namespace Utils
	{
		static std::unordered_map<std::string, ScriptFieldType> s_ScriptFieldTypeMap =
		{
			{"System.Single", ScriptFieldType::Float},
			{"System.Double", ScriptFieldType::Double},

			{"System.Boolean", ScriptFieldType::Bool},
			{"System.Char", ScriptFieldType::Char},
			{"System.Byte", ScriptFieldType::Byte},
			{"System.Int16", ScriptFieldType::Short},
			{"System.Int32", ScriptFieldType::Int},
			{"System.Int64", ScriptFieldType::Long},

			{"System.UInt16", ScriptFieldType::UShort},
			{"System.UInt32", ScriptFieldType::UInt},
			{"System.UInt64", ScriptFieldType::ULong},

			{"Ellie.Vector2", ScriptFieldType::Vector2},
			{"Ellie.Vector3", ScriptFieldType::Vector3},
			{"Ellie.Vector4", ScriptFieldType::Vector4},

			{"Ellie.Entity", ScriptFieldType::Entity}
		};

		static char* ReadBytes(const std::filesystem::path& filepath, uint32_t* outSize)
		{
			std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

			if (!stream)
			{
				// Failed to open the file
				return nullptr;
			}

			std::streampos end = stream.tellg();
			stream.seekg(0, std::ios::beg);
			uint32_t size = end - stream.tellg();

			if (size == 0)
			{
				// File is empty
				return nullptr;
			}

			char* buffer = new char[size];
			stream.read((char*)buffer, size);
			stream.close();

			*outSize = size;
			return buffer;
		}

		static void PrintAssemblyTypes(MonoAssembly* assembly)
		{
			MonoImage* image = mono_assembly_get_image(assembly);
			const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
			int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

			for (int32_t i = 0; i < numTypes; i++)
			{
				uint32_t cols[MONO_TYPEDEF_SIZE];
				mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

				const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
				const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

				printf("%s.%s\n", nameSpace, name);
			}
		}

		static MonoAssembly* LoadMonoAssembly(const std::filesystem::path& assemblyPath)
		{
			uint32_t fileSize = 0;
			char* fileData = ReadBytes(assemblyPath, &fileSize);

			// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
			MonoImageOpenStatus status;
			MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

			if (status != MONO_IMAGE_OK)
			{
				const char* errorMessage = mono_image_strerror(status);
				// Log some error message using the errorMessage data
				return nullptr;
			}

			MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.string().c_str(), &status, 0);
			mono_image_close(image);

			// Don't forget to free the file data
			delete[] fileData;

			return assembly;
		}

		static ScriptFieldType MonoTypeToScriptFieldType(MonoType* type)
		{
			std::string typeName = mono_type_get_name(type);
			//EE_WARN(typeName);

			auto it = s_ScriptFieldTypeMap.find(typeName);
			if (it == s_ScriptFieldTypeMap.end())
			{
				return ScriptFieldType::None;
			}

			return it->second;
		}

		static std::string ScriptFieldTypeToString(ScriptFieldType type)
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
	}

	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssembly = nullptr;
		MonoImage* CoreAssemblyImage = nullptr;
		
		MonoAssembly* AppAssembly = nullptr;
		MonoImage* AppAssemblyImage = nullptr;

		ScriptClass EntityClass;

		std::unordered_map<std::string, Ref<ScriptClass>> EntityClasses;
		std::unordered_map<UUID, Ref<ScriptInstance>> EntityInstances;

		// Runtime
		Scene* SceneContext = nullptr;
	};

	static ScriptEngineData* s_Data = nullptr;

	void ScriptEngine::Init()
	{
		s_Data = new ScriptEngineData();
		
		InitMono();
		LoadAssembly("Resources/Scripts/Ellie-ScriptCore.dll");
		LoadAppAssembly("SandboxProject/Assets/Scripts/Binaries/Sandbox.dll");

		LoadAssemblyClasses();

		ScriptGlue::RegisterComponents();
		ScriptGlue::RegisterFunctions();

		// Retrieve and instantiate class with constructor
		s_Data->EntityClass = ScriptClass("Ellie", "Entity", true);
#if 0
		MonoObject* instance = s_Data->EntityClass.Instantiate();

		// call methods without params
		MonoMethod* printMethod = s_Data->EntityClass.GetMethod("PrintCustomMessage", 0);
		s_Data->EntityClass.InvokeMethod(instance, printMethod);

		// call method with 1 param
		int value = 5;
		void* param = &value;
		MonoMethod* printInt = s_Data->EntityClass.GetMethod("PrintInt", 1);
		s_Data->EntityClass.InvokeMethod(instance, printInt, &param);

		// call method with multiple params
		int value1 = 500;
		int value2 = 64;
		void* params[2] =
		{
			&value1,
			&value2
		};
		MonoMethod* printInts = s_Data->EntityClass.GetMethod("PrintInts", 2);
		s_Data->EntityClass.InvokeMethod(instance, printInts, params);

		// string
		MonoString* monoStr = mono_string_new(s_Data->AppDomain, "Hello world from C++");
		void* strParam = monoStr;
		MonoMethod* printCustomMessageWithParams = s_Data->EntityClass.GetMethod("PrintCustomMessageWithParams", 1);
		s_Data->EntityClass.InvokeMethod(instance, printCustomMessageWithParams, &strParam);
#endif
	}

	void ScriptEngine::Shutdown()
	{
		ShutdownMono();
		delete s_Data;
	}

	void ScriptEngine::OnRuntimeStart(Scene* scene)
	{
		s_Data->SceneContext = scene;
	}

	void ScriptEngine::OnRuntimeStop()
	{
		s_Data->SceneContext = nullptr;
		s_Data->EntityInstances.clear();
	}

	void ScriptEngine::OnCreateEntity(Entity entity)
	{
		const auto& sc = entity.GetComponent<ScriptComponent>();
		if (ScriptEngine::EntityClassExists(sc.ClassName))
		{
			Ref<ScriptInstance> instance = std::make_shared<ScriptInstance>(s_Data->EntityClasses[sc.ClassName], entity);
			s_Data->EntityInstances[entity.GetUUID()] = instance;
			instance->InvokeOnCreate();
		}
	}

	void ScriptEngine::OnUpdateEntity(Entity entity, Timestep ts)
	{
		UUID entityUUID = entity.GetUUID();

		EE_ASSERT(s_Data->EntityInstances.find(entityUUID) != s_Data->EntityInstances.end(), "Could not find entity in EntityInstances");

		Ref<ScriptInstance> instance = s_Data->EntityInstances[entityUUID];
		instance->InvokeOnUpdate((float)ts);
	}

	void ScriptEngine::InitMono()
	{
		mono_set_assemblies_path("mono/lib");

		MonoDomain* rootDomain = mono_jit_init("EllieJITRuntime");
		EE_CORE_ASSERT(rootDomain, "RootDomain is null!");

		s_Data->RootDomain = rootDomain;
	}

	void ScriptEngine::ShutdownMono()
	{
		s_Data->AppDomain = nullptr;
		s_Data->RootDomain = nullptr;
	}

	void ScriptEngine::LoadAssembly(const std::filesystem::path& filepath)
	{
		s_Data->AppDomain = mono_domain_create_appdomain("EllieScriptRuntime", nullptr);
		mono_domain_set(s_Data->AppDomain, true);

		// Move this later
		s_Data->CoreAssembly = Utils::LoadMonoAssembly(filepath);
		s_Data->CoreAssemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);
		// Utils::PrintAssemblyTypes(s_Data->CoreAssembly);
	}
	
	void ScriptEngine::LoadAppAssembly(const std::filesystem::path& filepath)
	{
		// Move this later
		s_Data->AppAssembly = Utils::LoadMonoAssembly(filepath);
		s_Data->AppAssemblyImage = mono_assembly_get_image(s_Data->AppAssembly);
		// Utils::PrintAssemblyTypes(s_Data->AppAssembly);
	}

	Scene* ScriptEngine::GetSceneContext()
	{
		return s_Data->SceneContext;
	}

	std::unordered_map<std::string, Ref<ScriptClass>> ScriptEngine::GetEntityClasses()
	{
		return s_Data->EntityClasses;
	}

	bool ScriptEngine::EntityClassExists(const std::string& fullname)
	{
		if (s_Data->EntityClasses.find(fullname) != s_Data->EntityClasses.end())
		{
			return true;
		}

		return false;
	}

	MonoObject* ScriptEngine::InstantiateClass(MonoClass* monoClass)
	{
		MonoObject* instance = mono_object_new(s_Data->AppDomain, monoClass);
		mono_runtime_object_init(instance); // call constructor
		return instance;
	}

	void ScriptEngine::LoadAssemblyClasses()
	{
		s_Data->EntityClasses.clear();

		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(s_Data->AppAssemblyImage, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		std::string fullname;
		MonoClass* entityClass = mono_class_from_name(s_Data->CoreAssemblyImage, "Ellie", "Entity");

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* className = mono_metadata_string_heap(s_Data->AppAssemblyImage, cols[MONO_TYPEDEF_NAME]);

			if (strlen(nameSpace) != 0)
			{
				fullname = fmt::format("{}.{}", nameSpace, className);
			}
			else
			{
				fullname = className;
			}

			MonoClass* monoClass = mono_class_from_name(s_Data->AppAssemblyImage, nameSpace, className);
			if (monoClass == entityClass)
			{
				continue;
			}

			bool isEntity = mono_class_is_subclass_of(monoClass, entityClass, false);
			if (!isEntity)
			{
				continue;
			}

			Ref<ScriptClass> scriptClass = std::make_shared<ScriptClass>(nameSpace, className);
			s_Data->EntityClasses[fullname] = scriptClass;

			void* iter = nullptr;
			EE_TRACE(className);
			while (MonoClassField* field = mono_class_get_fields(monoClass, &iter))
			{
				std::string fieldName = mono_field_get_name(field);
				uint32_t flags = mono_field_get_flags(field);

				if (flags & FIELD_ATTRIBUTE_PUBLIC)
				{
					ScriptFieldType scriptFieldType = Utils::MonoTypeToScriptFieldType(mono_field_get_type(field));
					std::string scriptTypeName = Utils::ScriptFieldTypeToString(scriptFieldType);
					scriptClass->m_PublicFields[fieldName] = { scriptFieldType, fieldName, field };
				}
			}
		}
	}

	MonoImage* ScriptEngine::GetAssemblyCoreImage()
	{
		return s_Data->CoreAssemblyImage;
	}

	Ref<ScriptInstance> ScriptEngine::GetScriptInstanceFromUUID(UUID id)
	{
		auto it = s_Data->EntityInstances.find(id);
		if (it == s_Data->EntityInstances.end())
		{
			return nullptr;
		}

		return it->second;
	}

	ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className, bool isCore) : m_ClassName(className), m_ClassNamespace(classNamespace)
	{
		m_monoClass = mono_class_from_name(isCore ? s_Data->CoreAssemblyImage : s_Data->AppAssemblyImage, classNamespace.c_str(), className.c_str());
	}

	MonoObject* ScriptClass::Instantiate()
	{
		return ScriptEngine::InstantiateClass(m_monoClass);
	}

	MonoMethod* ScriptClass::GetMethod(const std::string& name, int parameterCount)
	{
		return mono_class_get_method_from_name(m_monoClass, name.c_str(), parameterCount);
	}

	MonoObject* ScriptClass::InvokeMethod(MonoObject* instance, MonoMethod* method, void** params)
	{
		return mono_runtime_invoke(method, instance, params, nullptr);
	}

	ScriptInstance::ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity) : m_ScriptClass(scriptClass)
	{
		m_Instance = m_ScriptClass->Instantiate();

		m_Constructor = s_Data->EntityClass.GetMethod(".ctor", 1);
		m_OnCreateMethod = m_ScriptClass->GetMethod("OnCreate", 0);
		m_OnUpdateMethod = m_ScriptClass->GetMethod("OnUpdate", 1);

		UUID entityID = entity.GetUUID();
		void* param = &entityID;
		m_ScriptClass->InvokeMethod(m_Instance, m_Constructor, &param);
	}

	void ScriptInstance::InvokeOnCreate()
	{
		if(m_OnCreateMethod)
		{
			m_ScriptClass->InvokeMethod(m_Instance, m_OnCreateMethod);
		}
	}

	void ScriptInstance::InvokeOnUpdate(float ts)
	{
		if(m_OnUpdateMethod)
		{
			void* param = &ts;
			m_ScriptClass->InvokeMethod(m_Instance, m_OnUpdateMethod, &param);
		}
	}

	bool ScriptInstance::GetFieldValueInternal(const std::string& fieldName, void* buffer)
	{
		const auto& fields = m_ScriptClass->GetPublicFields();

		auto it = fields.find(fieldName);
		if (it == fields.end())
		{
			return false;
		}

		const ScriptFields& type = it->second;
		mono_field_get_value(m_Instance, type.field, buffer);
		return true;
	}

	bool ScriptInstance::SetFieldValueInternal(const std::string& fieldName, const void* data)
	{
		const auto& fields = m_ScriptClass->GetPublicFields();

		auto it = fields.find(fieldName);
		if (it == fields.end())
		{
			return false;
		}

		const ScriptFields& type = it->second;
		mono_field_set_value(m_Instance, type.field, (void*)data);
		return true;
	}

}