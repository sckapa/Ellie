#include "eepch.h"
#include "ScriptGlue.h"
#include "glm/glm.hpp"

#include "mono/metadata/object.h"
#include "mono/metadata/reflection.h"

#include "Ellie/Scene/Scene.h"
#include "Ellie/Core/UUID.h"
#include "Ellie/Scripting/ScriptEngine.h"

#include "Ellie/Core/KeyCodes.h"
#include "Ellie/Core/Input.h"

namespace Ellie {

#define EE_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Ellie.InternalCalls::" #Name, Name)

	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFuncs;

	static bool Entity_HasComponent(UUID entityID, MonoReflectionType* componentType)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		EE_ASSERT(scene, "Scene is NULL");
		Entity entity = scene->GetEntityByUUID(entityID);
		EE_ASSERT(entity, "Entity is NULL");

		MonoType* monoComponentType = mono_reflection_type_get_type(componentType);
		EE_ASSERT(monoComponentType, "Mono Component type is NULL");
		return s_EntityHasComponentFuncs[monoComponentType](entity);
	}
	
	static uint64_t Entity_FindEntityByName(MonoString* name)
	{
		char* cStr = mono_string_to_utf8(name);

		Scene* scene = ScriptEngine::GetSceneContext();
		EE_ASSERT(scene, "Scene is NULL");
		Entity entity = scene->GetEntityByName(cStr);

		mono_free(cStr);

		if (!entity.IsValid())
		{
			return 0;
		}

		return entity.GetUUID();
	}
	
	static MonoObject* GetScriptInstance(UUID uuid)
	{
		return ScriptEngine::GetManagedInstance(uuid);
	}
	
	static void TransformComponent_GetTranslation(UUID entityID, glm::vec3* outTranslation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		*outTranslation = scene->GetEntityByUUID(entityID).GetComponent<TransformComponent>().Translation;
	}
	
	static void TransformComponent_SetTranslation(UUID entityID, glm::vec3* translation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		scene->GetEntityByUUID(entityID).GetComponent<TransformComponent>().Translation = *translation;
	}
	
	static bool Input_IsKeyDown(KeyCode keycode)
	{
		return Input::IsKeyPressed(keycode);
	}

	template<typename Component>
	static void RegisterComponent()
	{
		std::string_view typeName = typeid(Component).name();
		size_t pos = typeName.find_last_of(':') + 1;
		std::string_view structName = typeName.substr(pos);
		std::string managedTypename = fmt::format("Ellie.{}", structName);

		MonoType* managedType = mono_reflection_type_from_name(managedTypename.data(), ScriptEngine::GetAssemblyCoreImage());
		EE_ASSERT(managedType, "Managed Type is NULL");
		s_EntityHasComponentFuncs[managedType] = [](Entity entity) { return entity.HasComponent<Component>(); };
	}

	void ScriptGlue::RegisterComponents()
	{
		RegisterComponent<TransformComponent>();
	}

	void ScriptGlue::RegisterFunctions()
	{
		// Internal calls
		EE_ADD_INTERNAL_CALL(Entity_HasComponent);
		EE_ADD_INTERNAL_CALL(Entity_FindEntityByName);

		EE_ADD_INTERNAL_CALL(GetScriptInstance);

		EE_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		EE_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);

		EE_ADD_INTERNAL_CALL(Input_IsKeyDown);
	}
}