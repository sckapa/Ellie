#pragma once

#include "entt.hpp"
#include "Ellie/Core/Timestep.h"

namespace Ellie {

	class Scene
	{
	public:
		Scene();
		~Scene();

		entt::entity CreateEntity();
		entt::registry& Reg() { return m_Registry; }
		
		void OnUpdate(Timestep ts);

	private:
		entt::registry m_Registry;
	};

}

