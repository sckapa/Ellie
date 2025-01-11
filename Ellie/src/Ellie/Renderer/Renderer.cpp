#include "eepch.h"
#include "Renderer.h"

namespace Ellie {

	void Renderer::BeginScene()
	{
	}

	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderCommands::DrawIndexed(vertexArray);
	}

}