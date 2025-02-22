#include "eepch.h"
#include "OpenGLFrameBuffer.h"
#include "glad/glad.h"

namespace Ellie {

	namespace Utils {

		static GLenum TextureTarget(bool multisampled)
		{
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTextures(bool multiSampled, uint32_t* outID, uint32_t count)
		{
			glCreateTextures(TextureTarget(multiSampled), count, outID);
		}

		static void BindTextures(bool multiSampled, uint32_t id)
		{
			glBindTexture(TextureTarget(multiSampled), id);
		}

		static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
		{
			bool multiSampled = samples > 1;
			if (multiSampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multiSampled), id, 0);
			}
		}

		static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool multiSampled = samples > 1;
			if (multiSampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

				glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multiSampled), id, 0);
			}
		}

		static bool IsDepthFormat(FrameBufferTextureFormat format)
		{
			switch (format)
			{
				case FrameBufferTextureFormat::DEPTH24STENCIL8: return true;
			}

			return false;
		}

		static GLenum EllieFBTextureFormatToGL(FrameBufferTextureFormat format)
		{
			switch (format)
			{
				case FrameBufferTextureFormat::RGBA8:
				{
					return GL_RGBA8;
				}
				case FrameBufferTextureFormat::RED_INTEGER:
				{
					return GL_RED_INTEGER;
				}
			}

			EE_CORE_ASSERT(false, "Unidentified format");
			return 0;
		}

	}

	OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec) : m_spec(spec)
	{
		for (auto format : m_spec.Attachments.Attachments)
		{
			if (!Utils::IsDepthFormat(format))
			{
				m_ColorAttachentsSpecifications.emplace_back(format);
			}
			else
			{
				m_DepthAttachementSpecifications = format;
			}
		}

		Invalidate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
	}

	const void OpenGLFrameBuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_spec.width, m_spec.height);
	}

	const void OpenGLFrameBuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > 8192 || height > 8192)
		{
			EE_CORE_WARN("Attempted to resize frame buffer to {0}, {1}", width, height);
			return;
		}

		m_spec.width = width;
		m_spec.height = height;

		Invalidate();
	}

	int OpenGLFrameBuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		EE_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "");

		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);

		return pixelData;
	}

	void OpenGLFrameBuffer::ClearAttachment(uint32_t attachmentIndex, int value)
	{
		EE_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "");

		auto& spec = m_ColorAttachentsSpecifications[attachmentIndex];

		glClearTexImage(m_ColorAttachments[attachmentIndex], 0, Utils::EllieFBTextureFormatToGL(spec.TextureFormat), GL_INT, &value);
	}

	void OpenGLFrameBuffer::Invalidate()
	{
		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);

			m_ColorAttachments.clear();
			m_DepthAttachment = 0;
		}

		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		bool multiSample = m_spec.samples > 1;

		// Attachments
		if (m_ColorAttachentsSpecifications.size())
		{
			m_ColorAttachments.resize(m_ColorAttachentsSpecifications.size());
			Utils::CreateTextures(multiSample, m_ColorAttachments.data(), m_ColorAttachments.size());

			for (int i = 0;i < m_ColorAttachments.size();i++)
			{
				Utils::BindTextures(multiSample, m_ColorAttachments[i]);

				switch (m_ColorAttachentsSpecifications[i].TextureFormat)
				{
					case FrameBufferTextureFormat::RGBA8:
					{
						Utils::AttachColorTexture(m_ColorAttachments[i], m_spec.samples, GL_RGBA8, GL_RGBA, m_spec.width, m_spec.height, i);
						break;
					}
					case FrameBufferTextureFormat::RED_INTEGER:
					{
						Utils::AttachColorTexture(m_ColorAttachments[i], m_spec.samples, GL_R32I, GL_RED_INTEGER, m_spec.width, m_spec.height, i);
						break;
					}
				}
			}
		}

		if (m_DepthAttachementSpecifications.TextureFormat != FrameBufferTextureFormat::NONE)
		{
			Utils::CreateTextures(multiSample, &m_DepthAttachment, 1);
			Utils::BindTextures(multiSample, m_DepthAttachment);

			switch (m_DepthAttachementSpecifications.TextureFormat)
			{
				case FrameBufferTextureFormat::DEPTH24STENCIL8:
				{
					Utils::AttachDepthTexture(m_DepthAttachment, m_spec.samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_spec.width, m_spec.height);
					break;
				}
			}
		}

		if (m_ColorAttachments.size() > 1)
		{
			EE_ASSERT(m_ColorAttachments.size() <= 4, "");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_ColorAttachments.size(), buffers);
		}
		else if (m_ColorAttachments.size() == 0)
		{
			glDrawBuffer(GL_NONE);
		}

		EE_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Frame Buffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

}