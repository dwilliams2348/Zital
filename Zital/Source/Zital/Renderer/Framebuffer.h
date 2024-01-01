#pragma once

#include "Zital/Core/Core.h"

namespace Zital
{

	enum class FramebufferTextureFormat
	{
		None = 0,

		//color
		RGBA8,
		RED_INTEGER,

		//depth/stencli
		DEPTH24STENCIL8,

		//defaults
		Depth = DEPTH24STENCIL8
	};

	struct FramebufferTextureSpecification
	{
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat _format)
			: TextureFormat(_format) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
		//add filtering and wrap
	};

	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> _attachments)
			: Attachments(_attachments) {}

		std::vector<FramebufferTextureSpecification> Attachments;
	};

	struct FramebufferProperties
	{
		uint32_t Width, Height;
		FramebufferAttachmentSpecification Attachments;
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t _width, uint32_t _height) = 0;
		virtual int ReadPixel(uint32_t _attachmentIndex, int x, int y) = 0;

		virtual void ClearAttachment(uint32_t _attachmentIndex, int _value) = 0;

		virtual uint32_t GetColorAttachmentRendererID(uint32_t _index = 0) const = 0;

		//virtual FramebufferProperties& GetProperties() = 0;
		virtual const FramebufferProperties& GetProperties() const = 0;

		static Ref<Framebuffer> Create(const FramebufferProperties& _props);

	};

}