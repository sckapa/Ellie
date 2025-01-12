#pragma once

#include "RenderCommands.h"
#include "OrthographicCamera.h"
#include "Shader.h"
#include <glm/glm.hpp>

namespace Ellie {

	class Renderer
	{
	public:
		static void BeginScene(OrthographicCamera& orthoCam);
		static void EndScene();

		static void Submit(const std::shared_ptr<Shader> shader, const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* sceneData;
	};

}