#pragma once

#include "RendererAPI.h"

namespace Ellie {

	class RenderCommands
	{
	public:
		static void Init()
		{
			s_RendererAPI->Init();
		}

		static void WindowResize(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->WindowResize(x, y, width, height);
		}

		static void SetClearColor(const glm::vec4& color)
		{
			s_RendererAPI->SetClearColor(color);
		}

		static void Clear()
		{
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0)
		{
			s_RendererAPI->DrawIndexed(vertexArray, indexCount);
		}

	private:
		static RendererAPI* s_RendererAPI;
	};

}