#pragma once

#include "entt.hpp"
#include "Ellie/Core/Timestep.h"

namespace Ellie {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(std::string name = std::string());
		void DestroyEntity(Entity entity);
		
		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);

		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

	private:
		entt::registry m_Registry;

		uint32_t m_ViewportWidth = 0.0f, m_ViewportHeight = 0.0f;

		friend class Entity;
		friend class SceneHierarchyPanel;
	};

}

