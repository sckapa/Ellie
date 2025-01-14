#pragma once

#include "Ellie/Renderer/VertexArray.h"

namespace Ellie{

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffers(const Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		std::vector<Ref<VertexBuffer>> GetVertexBuffers() { return m_VertexBuffers; }
		Ref<IndexBuffer> GetIndexBuffer() { return m_IndexBuffer; }

	private:
		uint32_t m_RendererID;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};

}

