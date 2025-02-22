#pragma once

#include "Ellie/Core/Core.h"

namespace Ellie {

	enum class FrameBufferTextureFormat
	{
		NONE=0,

		// Color
		RGBA8,
		RED_INTEGER,

		// Depth/Stencil
		DEPTH24STENCIL8,

		// Defaults
		Depth = DEPTH24STENCIL8
	};

	struct FrameBufferTextureSpecifications
	{
		FrameBufferTextureSpecifications() = default;
		FrameBufferTextureSpecifications(FrameBufferTextureFormat format) : TextureFormat(format) {}

		FrameBufferTextureFormat TextureFormat = FrameBufferTextureFormat::NONE;
	};

	struct FrameBufferAttachmentSpecifications
	{
		FrameBufferAttachmentSpecifications() = default;
		FrameBufferAttachmentSpecifications(std::initializer_list<FrameBufferTextureFormat> attachments)
			: Attachments(attachments) {}

		std::vector<FrameBufferTextureFormat> Attachments;
	};

	struct FrameBufferSpecification
	{
		uint32_t width, height;
		uint32_t samples = 1;
		FrameBufferAttachmentSpecifications Attachments;
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
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;
		virtual uint32_t GetDepthAttachmentRendererID() = 0;

		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
	};

}

