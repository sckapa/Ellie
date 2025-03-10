#include "eepch.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "Renderer2D.h"

namespace Ellie {

	Renderer::SceneData* Renderer::sceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RenderCommands::Init();
		Renderer2D::Init();
	}

	void Renderer::WindowResize(uint32_t width, uint32_t height)
	{
		RenderCommands::WindowResize(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& orthoCam)
	{
		sceneData->ViewProjectionMatrix = orthoCam.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<Shader> shader, const Ref<VertexArray>& vertexArray, glm::mat4 transform)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", sceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommands::DrawIndexed(vertexArray);
	}

}