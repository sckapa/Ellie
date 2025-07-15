#pragma once

#include "Ellie/Scene/Scene.h"
#include "Ellie/Scene/Entity.h"

#include <filesystem>
#include <string>
#include <map>

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
	typedef struct _MonoClassField MonoClassField;
}

namespace Ellie {

	enum class ScriptFieldType
	{
		None = 0,
		Float, Double,
		Bool, Byte, Short, Int, Long, Char,
		UShort, UInt, ULong,
		Vector2, Vector3, Vector4,
		Entity
	};

	struct ScriptFields
	{
		ScriptFieldType type;
		std::string name;
		MonoClassField* field;
	};
	
	struct ScriptFieldInstance
	{
		ScriptFields field;

		template<typename T>
		T GetValue()
		{
			static_assert(sizeof(T) <= 16, "Type too large!");
			return *(T*)m_Buffer;
		}

		template<typename T>
		void SetValue(T data)
		{
			static_assert(sizeof(T) <= 16, "Type too large!");
			memcpy(m_Buffer, &data, sizeof(T));
		}

	private:
		uint8_t m_Buffer[16];

		friend class ScriptEngine;
	};

	using ScriptFieldMap = std::unordered_map<std::string, ScriptFieldInstance>;

	class ScriptClass
	{
	public:
		ScriptClass() = default;
		ScriptClass(const std::string& classNamespace, const std::string& className, bool isCore = false);

		MonoObject* Instantiate();
		MonoMethod* GetMethod(const std::string& name, int parameterCount);
		MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);

		std::map<std::string, ScriptFields> GetPublicFields() { return m_PublicFields; }
	private:
		std::string m_ClassName;
		std::string m_ClassNamespace;

		std::map<std::string, ScriptFields> m_PublicFields;

		MonoClass* m_monoClass = nullptr;

		friend class ScriptEngine;
	};

	class ScriptInstance
	{
	public:
		ScriptInstance(Ref<ScriptClass> scriptClass, Entity entity);

		void InvokeOnCreate();
		void InvokeOnUpdate(float ts);

		Ref<ScriptClass> GetScriptClass() { return m_ScriptClass; }

		template<typename T>
		T GetFieldValue(const std::string& fieldName)
		{
			static_assert(sizeof(T) <= 16, "Type too large!");

			bool success = GetFieldValueInternal(fieldName, s_FieldValueBuffer);
			if (!success)
			{
				return T();
			}

			return *(T*)s_FieldValueBuffer;
		}
		
		template<typename T>
		void SetFieldValue(const std::string& fieldName, T data)
		{
			static_assert(sizeof(T) <= 16, "Type too large!");
			SetFieldValueInternal(fieldName, &data);
		}

		MonoObject* GetManagedObject();

	private:
		bool GetFieldValueInternal(const std::string& fieldName, void* buffer);
		bool SetFieldValueInternal(const std::string& fieldName, const void* data);

		Ref<ScriptClass> m_ScriptClass;
			
		MonoObject* m_Instance = nullptr;
		MonoMethod* m_Constructor = nullptr;
		MonoMethod* m_OnCreateMethod = nullptr;
		MonoMethod* m_OnUpdateMethod = nullptr;

		inline static char s_FieldValueBuffer[16];

		friend class ScriptEngine;
	};

	class ScriptEngine
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnRuntimeStart(Scene* scene);
		static void OnRuntimeStop();

		static void OnCreateEntity(Entity entity);
		static void OnUpdateEntity(Entity entity, Timestep ts);

		static void LoadAssembly(const std::filesystem::path& filepath);
		static void LoadAppAssembly(const std::filesystem::path& filepath);

		static Scene* GetSceneContext();
		static Ref<ScriptClass> GetEntityClass(std::string& name);
		static std::unordered_map<std::string, Ref<ScriptClass>> GetEntityClasses();
		static bool EntityClassExists(const std::string& fullname);

		static MonoImage* GetAssemblyCoreImage();

		static Ref<ScriptInstance> GetScriptInstanceFromUUID(UUID id);

		static ScriptFieldMap& GetScriptFieldMap(Entity entity);

		static MonoObject* GetManagedInstance(UUID uuid);
	private:
		static void InitMono();
		static void ShutdownMono();

		static MonoObject* InstantiateClass(MonoClass* monoClass);
		static void LoadAssemblyClasses();

		friend class ScriptClass;
		friend class ScriptGlue;
	};

}

