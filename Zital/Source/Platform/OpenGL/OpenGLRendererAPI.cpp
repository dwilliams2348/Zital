#include "ZTpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Zital
{
	void OpenGLRendererAPI::Init()
	{
		ZT_PROFILE_FUNCTION();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

    void OpenGLRendererAPI::SetViewport(uint32_t _x, uint32_t _y, uint32_t _width, uint32_t _height)
    {
		glViewport(_x, _y, _width, _height);
    }

    void OpenGLRendererAPI::SetClearColor(const glm::vec4& _color)
	{
		glClearColor(_color.r, _color.g, _color.b, _color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& _vertexArray, uint32_t _indexCount)
	{
		uint32_t count = _indexCount ? _vertexArray->GetIndexBuffer()->GetIndexCount() : _indexCount;
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}