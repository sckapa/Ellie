#pragma once

#include <entt.hpp>
#include "Ellie/Core/Timestep.h"
#include "Ellie/Renderer/EditorCamera.h"
#include "Ellie/Core/UUID.h"

class b2World;

namespace Ellie {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		static Ref<Scene> Copy(Ref<Scene> other);

		Entity CreateEntity(std::string name = std::string());
		Entity CreateEntityWithUUID(UUID id, std::string name = std::string());
		void DestroyEntity(Entity entity);

		void OnRuntimeStart();
		void OnRuntimeStop();
		
		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& editorCam);

		void OnViewportResize(uint32_t width, uint32_t height);

		void DuplicateEntity(Entity& entity);

		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

		Entity GetEntityByUUID(UUID entityID);
		Entity GetPrimaryCameraEntity();

	private:
		entt::registry m_Registry;

		uint32_t m_ViewportWidth = 0.0f, m_ViewportHeight = 0.0f;

		b2World* m_PhysicsWorld = nullptr;

		std::unordered_map<UUID, entt::entity> EntityMap;

		friend class Entity;
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;
	};
}
