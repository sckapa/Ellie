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
		
		void OnUpdate(Timestep ts);

	private:
		entt::registry m_Registry;

		friend class Entity;
	};

}

