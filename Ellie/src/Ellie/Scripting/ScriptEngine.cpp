#include "eepch.h"
#include "ScriptEngine.h"
#include "glm/glm.hpp"

#include "ScriptGlue.h"
#include "mono/metadata/object.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/appdomain.h>

namespace Ellie {

	namespace Utils
	{
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
	}

	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssembly = nullptr;
		MonoImage* CoreAssemblyImage = nullptr;

		ScriptClass EntityClass;
	};

	static ScriptEngineData* s_Data = nullptr;

	void ScriptEngine::Init()
	{
		s_Data = new ScriptEngineData();
		
		InitMono();
		LoadAssembly("Resources/Scripts/Ellie-ScriptCore.dll");

		ScriptGlue::RegisterFunctions();

		// Retrieve and instantiate class with constructor
		s_Data->EntityClass = ScriptClass("Ellie", "Entity");
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
	}

	void ScriptEngine::Shutdown()
	{
		ShutdownMono();
		delete s_Data;
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

	MonoObject* ScriptEngine::InstantiateClass(MonoClass* monoClass)
	{
		MonoObject* instance = mono_object_new(s_Data->AppDomain, monoClass);
		mono_runtime_object_init(instance); // call constructor
		return instance;
	}

	ScriptClass::ScriptClass(const std::string& classNamespace, const std::string& className) : m_ClassName(className), m_ClassNamespace(classNamespace)
	{
		m_monoClass = mono_class_from_name(s_Data->CoreAssemblyImage, classNamespace.c_str(), className.c_str());
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

}