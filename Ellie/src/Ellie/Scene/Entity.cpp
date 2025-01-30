#include "eepch.h"
#include "Entity.h"


namespace Ellie {

	Entity::Entity(entt::entity entity, Scene* scene) : m_EntityHandle(entity), m_Scene(scene)
	{
	}

}