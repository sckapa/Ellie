#pragma once

#include "Ellie/Core/Core.h"

namespace Ellie{

	struct FrameBufferSpecification
	{
		uint32_t width, height;
		uint32_t samples = 1;
		bool swapChainTarget = false;
	};

	class FrameBuffer
	{
	public:
		virtual ~FrameBuffer() = default;
		virtual const FrameBufferSpecification& GetFrameBufferSpec() const = 0;

		virtual const void Bind() const = 0;
		virtual const void Unbind() const = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColorAttachmentRendererID() = 0;
		virtual uint32_t GetDepthAttachmentRendererID() = 0;

		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
	};

}

