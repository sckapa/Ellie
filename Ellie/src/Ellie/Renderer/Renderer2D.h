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

		static void Flush();

		static void DrawQuad(const glm::vec2 position, const glm::vec2 size, const glm::vec4 color);
		static void DrawQuad(const glm::vec3 position, const glm::vec2 size, const glm::vec4 color);
		static void DrawQuad(const glm::vec3 position, const glm::vec2 size, const Ref<Texture2D> texture);
		static void DrawQuad(const glm::vec2 position, const glm::vec2 size, const Ref<Texture2D> texture);

		static void DrawRotatedQuad(const glm::vec2 position, const glm::vec2 size, float rotationInRadians, const glm::vec4 color);
		static void DrawRotatedQuad(const glm::vec3 position, const glm::vec2 size, float rotationInRadians, const glm::vec4 color);
		static void DrawRotatedQuad(const glm::vec3 position, const glm::vec2 size, float rotationInRadians, const Ref<Texture2D> texture);
		static void DrawRotatedQuad(const glm::vec2 position, const glm::vec2 size, float rotationInRadians, const Ref<Texture2D> texture);
	};

}
