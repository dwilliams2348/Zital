#include "ZTpch.h"
#include "OpenGLUniformBuffer.h"

#include <glad/glad.h>

namespace Zital
{

	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t _size, uint32_t _binding)
	{
		glCreateBuffers(1, &mRendererID);
		glNamedBufferData(mRendererID, _size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, _binding, mRendererID);
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer()
	{
		glDeleteBuffers(1, &mRendererID);
	}

	void OpenGLUniformBuffer::SetData(const void* _data, uint32_t _size, uint32_t _offset)
	{
		glNamedBufferSubData(mRendererID, _offset, _size, _data);
	}

}