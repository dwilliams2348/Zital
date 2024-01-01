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

		virtual void Resize(uint32_t _width, uint32_t _height) override;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t _index = 0) const override { ZT_CORE_ASSERT(_index < mColorAttachments.size(), "Index is too large."); return mColorAttachments[_index]; }

		virtual const FramebufferProperties& GetProperties() const override { return mProperties; }

	private:
		uint32_t mRendererID = 0;
		FramebufferProperties mProperties;

		std::vector<FramebufferTextureSpecification> mColorAttachmentSpecifications;
		FramebufferTextureSpecification mDepthAttachmentSpecification = FramebufferTextureFormat::None;

		std::vector<uint32_t> mColorAttachments;
		uint32_t mDepthAttachment = 0;
	};

}