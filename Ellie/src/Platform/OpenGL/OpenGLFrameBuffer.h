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

		virtual uint32_t GetColorAttachmentRendererID() override { return m_ColorAttachment; }
		virtual uint32_t GetDepthAttachmentRendererID() override { return m_DepthAttachment; }

		void Invalidate();

		virtual const FrameBufferSpecification& GetFrameBufferSpec() const override { return m_spec; }

	private:
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;
		FrameBufferSpecification m_spec;
	};

}

