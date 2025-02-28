#pragma once

#include "Scene.h"
#include "entt.hpp"
#include "Ellie/Core/UUID.h"
#include "Ellie/Scene/Components.h"

namespace Ellie {

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity entity, Scene* scene);

		bool IsValid()
		{
			return m_EntityHandle != entt::null;
		}

		template<typename T>
		T& GetComponent()
		{
			EE_CORE_ASSERT(HasComponent<T>(), "Component does not exist!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			EE_CORE_ASSERT(!HasComponent<T>(), "Component already exists!");
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		void RemoveComponent()
		{
			EE_CORE_ASSERT(HasComponent<T>(), "Component does not exist!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.all_of<T>(m_EntityHandle);
		}

		UUID GetUUID() { return GetComponent<IDComponent>().ID; }

		operator uint32_t() const { return (uint32_t)m_EntityHandle; }
		operator entt::entity() const { return m_EntityHandle; }

		bool operator==(const Entity& entity) const 
		{ 
			return m_EntityHandle == entity.m_EntityHandle && m_Scene == entity.m_Scene;
		}

		bool operator!=(const Entity& entity)
		{
			return !(*this == entity);
		}

	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};

}

