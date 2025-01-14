#pragma once

#include "Ellie/Renderer/VertexArray.h"

namespace Ellie{

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffers(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

		std::vector<std::shared_ptr<VertexBuffer>> GetVertexBuffers() { return m_VertexBuffers; }
		std::shared_ptr<IndexBuffer> GetIndexBuffer() { return m_IndexBuffer; }

	private:
		uint32_t m_RendererID;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};

}

