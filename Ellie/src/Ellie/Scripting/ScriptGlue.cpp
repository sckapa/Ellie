#include "eepch.h"
#include "ScriptGlue.h"
#include "mono/metadata/object.h"
#include "glm/glm.hpp"

#include "Ellie/Scene/Scene.h"
#include "Ellie/Core/UUID.h"
#include "Ellie/Scripting/ScriptEngine.h"

#include "Ellie/Core/KeyCodes.h"
#include "Ellie/Core/Input.h"

namespace Ellie {

#define EE_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Ellie.InternalCalls::" #Name, Name)

	static void Normalize_Vector3(glm::vec3* parameter, glm::vec3* outParam)
	{
		*outParam = glm::normalize(*parameter);
	}
	
	static void Entity_GetTranslation(UUID entityID, glm::vec3* outTranslation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		*outTranslation = scene->GetEntityByUUID(entityID).GetComponent<TransformComponent>().Translation;
	}
	
	static void Entity_SetTranslation(UUID entityID, glm::vec3* translation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		scene->GetEntityByUUID(entityID).GetComponent<TransformComponent>().Translation = *translation;
	}
	
	static bool Input_IsKeyDown(KeyCode keycode)
	{
		return Input::IsKeyPressed(keycode);
	}

	void ScriptGlue::RegisterFunctions()
	{
		// Internal calls
		EE_ADD_INTERNAL_CALL(Normalize_Vector3);

		EE_ADD_INTERNAL_CALL(Entity_GetTranslation);
		EE_ADD_INTERNAL_CALL(Entity_SetTranslation);

		EE_ADD_INTERNAL_CALL(Input_IsKeyDown);
	}
}