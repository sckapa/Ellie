#pragma once

#include "OrthographicCamera.h"
#include "Texture.h"

namespace Ellie {

	class Renderer2D
	{
	public:
		static void Init();
		static void ShutDown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void DrawQuad(glm::vec2 position, glm::vec2 size, glm::vec4 color);
		static void DrawQuad(glm::vec3 position, glm::vec2 size, glm::vec4 color);

		static void DrawQuad(glm::vec3 position, glm::vec2 size, Ref<Texture2D> texture);
		static void DrawQuad(glm::vec2 position, glm::vec2 size, Ref<Texture2D> texture);
	};

}
