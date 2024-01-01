#include "ZTpch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Zital
{

	static const uint32_t sMaxFramebufferSize = 8192;
	
	namespace Utils
	{

		static GLenum TextureTarget(bool _multisampled)
		{
			return _multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTextures(bool _multisampled, uint32_t* _outID, uint32_t _count)
		{
			glCreateTextures(TextureTarget(_multisampled), _count, _outID);
		}

		static void BindTexture(bool _multisampled, uint32_t _id)
		{
			glBindTexture(TextureTarget(_multisampled), _id);
		}

		static void AttachColorTexture(uint32_t _id, int _samples, GLenum _format, uint32_t _width, uint32_t _height, int _index)
		{
			bool multisampled = _samples > 1;
			if (multisampled)
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, _samples, _format, _width, _height, GL_FALSE);
			}
			else
			{
				glTexImage2D(GL_TEXTURE_2D, 0, _format, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
				
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + _index, TextureTarget(multisampled), _id, 0);
		}

		static void AttachDepthTexture(uint32_t _id, int _samples, GLenum _format, GLenum _attachmentType, uint32_t _width, uint32_t _height)
		{
			bool multisampled = _samples > 1;
			if (multisampled)
			{
				glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, _samples, _format, _width, _height, GL_FALSE);
			}
			else
			{
				glTexStorage2D(GL_TEXTURE_2D, 1, _format, _width, _height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, _attachmentType, TextureTarget(multisampled), _id, 0);
		}

		static bool IsDepthFormat(FramebufferTextureFormat _format)
		{
			switch (_format)
			{
				case FramebufferTextureFormat::DEPTH24STENCIL8:	return true;
			}

			return false;
		}

	}

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferProperties& _props)
		:mProperties(_props)
	{
		for (auto format : mProperties.Attachments.Attachments)
		{
			if (!Utils::IsDepthFormat(format.TextureFormat))
				mColorAttachmentSpecifications.emplace_back(format);
			else
				mDepthAttachmentSpecification = format;
		}

		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &mRendererID);
		glDeleteTextures(mColorAttachments.size(), mColorAttachments.data());
		glDeleteTextures(1, &mDepthAttachment);
	}

	void OpenGLFramebuffer::Invalidate()
	{
		if (mRendererID)
		{
			glDeleteFramebuffers(1, &mRendererID);
			glDeleteTextures(mColorAttachments.size(), mColorAttachments.data());
			glDeleteTextures(1, &mDepthAttachment);

			mColorAttachments.clear();
			mDepthAttachment = 0;
		}

		glCreateFramebuffers(1, &mRendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, mRendererID);

		bool multisample = mProperties.Samples > 1;

		//attachments
		if (mColorAttachmentSpecifications.size())
		{
			mColorAttachments.resize(mColorAttachmentSpecifications.size());
			Utils::CreateTextures(multisample, mColorAttachments.data(), mColorAttachments.size());

			for (size_t i = 0; i < mColorAttachments.size(); i++)
			{
				Utils::BindTexture(multisample, mColorAttachments[i]);
				switch (mColorAttachmentSpecifications[i].TextureFormat)
				{
					case FramebufferTextureFormat::RGBA8:
						Utils::AttachColorTexture(mColorAttachments[i], mProperties.Samples, GL_RGBA8, mProperties.Width, mProperties.Height, i);
						break;
				}
			}
		}

		if (mDepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None)
		{
			Utils::CreateTextures(multisample, &mDepthAttachment, 1);
			Utils::BindTexture(multisample, mDepthAttachment);

			switch (mDepthAttachmentSpecification.TextureFormat)
			{
				case FramebufferTextureFormat::DEPTH24STENCIL8:
					Utils::AttachDepthTexture(mDepthAttachment, mProperties.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, mProperties.Width, mProperties.Height);
					break;
			}
		}

		if (mColorAttachments.size() > 1)
		{
			ZT_CORE_ASSERT(mColorAttachments.size() <= 4, "Only 4 color attachments are supported at this time");

			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(mColorAttachments.size(), buffers);
		}
		else if(mColorAttachments.empty())
		{
			//only for depth-pass
			glDrawBuffer(GL_NONE);
		}

		//glCreateTextures(GL_TEXTURE_2D, 1, &mColorAttachment);
		//glBindTexture(GL_TEXTURE_2D, mColorAttachment);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, mProperties.Width, mProperties.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorAttachment, 0);
		//
		//glCreateTextures(GL_TEXTURE_2D, 1, &mDepthAttachment);
		//glBindTexture(GL_TEXTURE_2D, mDepthAttachment);
		//
		//glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, mProperties.Width, mProperties.Height);
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mDepthAttachment, 0);

		ZT_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete.");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, mRendererID);
		glViewport(0, 0, mProperties.Width, mProperties.Height);
	}

	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t _width, uint32_t _height)
	{
		mProperties.Width = _width;
		mProperties.Height = _height;

		Invalidate();
	}

}