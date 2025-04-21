#include "eepch.h"
#include "ScriptEngine.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"

namespace Ellie {

	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;

		MonoAssembly* CoreAssembly = nullptr;
	};

	static ScriptEngineData* s_Data = nullptr;

	void ScriptEngine::Init()
	{
		s_Data = new ScriptEngineData();
		
		InitMono();
	}

	void ScriptEngine::Shutdown()
	{
		delete s_Data;
		ShutdownMono();
	}

	char* ReadBytes(const std::string& filepath, uint32_t* outSize)
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

	MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath)
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

		MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
		mono_image_close(image);

		// Don't forget to free the file data
		delete[] fileData;

		return assembly;
	}

	void PrintAssemblyTypes(MonoAssembly* assembly)
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

	void ScriptEngine::InitMono()
	{
		mono_set_assemblies_path("mono/lib");

		MonoDomain* rootDomain = mono_jit_init("EllieJITRuntime");
		EE_CORE_ASSERT(rootDomain, "RootDomain is null!");

		s_Data->RootDomain = rootDomain;

		s_Data->AppDomain = mono_domain_create_appdomain("EllieScriptRuntime", nullptr);
		mono_domain_set(s_Data->AppDomain, true);

		// Move this later
		s_Data->CoreAssembly = LoadCSharpAssembly("Resources/Scripts/Ellie-ScriptCore.dll");
		PrintAssemblyTypes(s_Data->CoreAssembly);

		MonoImage* image = mono_assembly_get_image(s_Data->CoreAssembly);
		MonoClass* monoClass = mono_class_from_name(image, "Ellie", "Main");

		MonoObject* instance = mono_object_new(s_Data->AppDomain, monoClass);
		mono_runtime_object_init(instance); // call constructor

		// call methods without params
		MonoMethod* printMethod = mono_class_get_method_from_name(monoClass, "PrintCustomMessage", 0);
		mono_runtime_invoke(printMethod, instance, nullptr, nullptr);

		// call method with 1 param
		int value = 5;
		void* param = &value;
		MonoMethod* printInt = mono_class_get_method_from_name(monoClass, "PrintInt", 1);
		mono_runtime_invoke(printInt, instance, &param, nullptr);

		// call method with multiple params
		int value1 = 500;
		int value2 = 64;
		void* params[2] =
		{
			&value1,
			&value2
		};
		MonoMethod* printInts = mono_class_get_method_from_name(monoClass, "PrintInts", 2);
		mono_runtime_invoke(printInts, instance, params, nullptr);

		// string
		MonoString* monoStr = mono_string_new(s_Data->AppDomain, "Hello world from C++");
		void* strParam = monoStr;
		MonoMethod* printCustomMessageWithParams = mono_class_get_method_from_name(monoClass, "PrintCustomMessageWithParams", 1);
		mono_runtime_invoke(printCustomMessageWithParams, instance, &strParam, nullptr);
	}

	void ScriptEngine::ShutdownMono()
	{
	}

}