#pragma once

#include "Zital/Core/Core.h"

namespace Zital
{

	struct FramebufferProperties
	{
		uint32_t Width, Height;
		//ramebufferFormat Format
		uint32_t Samples = 1;

		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void Resize(uint32_t _width, uint32_t _height) = 0;

		virtual uint32_t GetColorAttachmentRendererID() const = 0;

		//virtual FramebufferProperties& GetProperties() = 0;
		virtual const FramebufferProperties& GetProperties() const = 0;

		static Ref<Framebuffer> Create(const FramebufferProperties& _props);

	};

}