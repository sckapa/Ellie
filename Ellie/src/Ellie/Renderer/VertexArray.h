#pragma once

#include "Ellie/Renderer/Buffers.h"

namespace Ellie {

	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffers(const Ref<VertexBuffer>& vertexBuffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

		virtual std::vector<Ref<VertexBuffer>> GetVertexBuffers() = 0;
		virtual Ref<IndexBuffer> GetIndexBuffer() = 0;

		static VertexArray* Create();
	};

}