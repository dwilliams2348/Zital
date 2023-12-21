#pragma once

#include "Zital/Renderer/Framebuffer.h"

namespace Zital
{

	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferProperties& _props);
		virtual ~OpenGLFramebuffer();

		void Invalidate();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual uint32_t GetColorAttachmentRendererID() const override { return mColorAttachment; }

		virtual const FramebufferProperties& GetProperties() const override { return mProperties; }

	private:
		uint32_t mRendererID;
		uint32_t mColorAttachment, mDepthAttachment;
		FramebufferProperties mProperties;
	};

}