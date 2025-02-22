#pragma once

#include "Ellie/Core/Core.h"
#include "Ellie/Renderer/FrameBuffer.h"

namespace Ellie{

	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(const FrameBufferSpecification& spec);
		virtual ~OpenGLFrameBuffer();

		virtual const void Bind() const override;
		virtual const void Unbind() const override;

		virtual void Resize(uint32_t width, uint32_t height) override;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override { EE_CORE_ASSERT(m_ColorAttachments.size() > index, ""); return m_ColorAttachments[index]; }
		virtual uint32_t GetDepthAttachmentRendererID() override { return m_DepthAttachment; }

		void Invalidate();

		virtual const FrameBufferSpecification& GetFrameBufferSpec() const override { return m_spec; }

	private:
		uint32_t m_RendererID = 0;
		FrameBufferSpecification m_spec;

		std::vector<FrameBufferTextureSpecifications> m_ColorAttachentsSpecifications;
		FrameBufferTextureSpecifications m_DepthAttachementSpecifications = FrameBufferTextureFormat::NONE;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment = 0;
	};

}

