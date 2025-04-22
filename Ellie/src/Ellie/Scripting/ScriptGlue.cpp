#include "eepch.h"
#include "ScriptGlue.h"
#include "mono/metadata/object.h"
#include "glm/glm.hpp"

namespace Ellie {

#define EE_ADD_INTERNAL_CALL(Name) mono_add_internal_call("Ellie.InternalCalls::" #Name, Name)

	static void Normalize_Vector3(glm::vec3* parameter, glm::vec3* outParam)
	{
		*outParam = glm::normalize(*parameter);
	}

	void ScriptGlue::RegisterFunctions()
	{
		// Internal calls
		EE_ADD_INTERNAL_CALL(Normalize_Vector3);
	}
}