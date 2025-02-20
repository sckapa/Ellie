#pragma once

#include "OrthographicCamera.h"
#include "Texture.h"
#include "Ellie/Renderer/Camera.h"
#include "Ellie/Renderer/EditorCamera.h"

namespace Ellie {

	class Renderer2D
	{
	public:
		static void Init();
		static void ShutDown();

		static void BeginScene(const Camera& camera, glm::mat4 transform);
		static void BeginScene(const OrthographicCamera& camera);
		static void BeginScene(const EditorCamera& editorCamera);
		static void EndScene();

		static void Flush();

		static void DrawQuad(const glm::vec2 position, const glm::vec2 size, const glm::vec4 color);
		static void DrawQuad(const glm::vec3 position, const glm::vec2 size, const glm::vec4 color);
		static void DrawQuad(const glm::vec3 position, const glm::vec2 size, const Ref<Texture2D> texture);
		static void DrawQuad(const glm::vec2 position, const glm::vec2 size, const Ref<Texture2D> texture);

		static void DrawQuad(const glm::mat4 transform, const glm::vec4 color);
		static void DrawQuad(const glm::mat4 transform, const Ref<Texture2D> texture);

		static void DrawRotatedQuad(const glm::vec2 position, const glm::vec2 size, float rotationInRadians, const glm::vec4 color);
		static void DrawRotatedQuad(const glm::vec3 position, const glm::vec2 size, float rotationInRadians, const glm::vec4 color);
		static void DrawRotatedQuad(const glm::vec3 position, const glm::vec2 size, float rotationInRadians, const Ref<Texture2D> texture);
		static void DrawRotatedQuad(const glm::vec2 position, const glm::vec2 size, float rotationInRadians, const Ref<Texture2D> texture);

		struct Statistics
		{
			uint32_t QuadCount = 0;
			uint32_t DrawCount = 0;

			uint32_t GetQuadCount() { return QuadCount; }
			uint32_t GetDrawCount() { return DrawCount; }
		};
		static Statistics GetStatistics();
		static void ResetStats();

	private:
		static void FlushAndReset();
	};

}
