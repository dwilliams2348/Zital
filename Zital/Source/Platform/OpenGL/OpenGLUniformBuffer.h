#pragma once

#include "Zital/Renderer/UniformBuffer.h"

namespace Zital
{

	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(uint32_t _size, uint32_t _binding);
		virtual ~OpenGLUniformBuffer();

		virtual void SetData(const void* _data, uint32_t _size, uint32_t _offset = 0);

	private:
		uint32_t mRendererID = 0;
	};

}