#pragma once

#include "Scene.h"
#include "entt.hpp"

namespace Ellie {

	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity entity, Scene* scene);

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
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
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

	private:
		entt::entity m_EntityHandle = entt::null;
		Scene* m_Scene = nullptr;
	};

}

