#include "ZTpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Zital
{

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType _type)
	{
		switch (_type)
		{
			case Zital::ShaderDataType::Float:		return GL_FLOAT;
			case Zital::ShaderDataType::Float2:		return GL_FLOAT;
			case Zital::ShaderDataType::Float3:		return GL_FLOAT;
			case Zital::ShaderDataType::Float4:		return GL_FLOAT;
			case Zital::ShaderDataType::Mat3:		return GL_FLOAT;
			case Zital::ShaderDataType::Mat4:		return GL_FLOAT;
			case Zital::ShaderDataType::Int:		return GL_INT;
			case Zital::ShaderDataType::Int2:		return GL_INT;
			case Zital::ShaderDataType::Int3:		return GL_INT;
			case Zital::ShaderDataType::Int4:		return GL_INT;
			case Zital::ShaderDataType::Bool:		return GL_BOOL;
		}

		ZT_CORE_ASSERT (false, "Unknown shader data type.");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		ZT_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &mRendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		ZT_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &mRendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		ZT_PROFILE_FUNCTION();

		glBindVertexArray(mRendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		ZT_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& _buffer)
	{
		ZT_PROFILE_FUNCTION();

		ZT_CORE_ASSERT(_buffer->GetLayout().GetElements().size(), "Vertex buffer has no layout, cannot draw");
		
		glBindVertexArray(mRendererID);
		_buffer->Bind();

		uint32_t index = 0;
		const auto& layout = _buffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}

		mVertexBuffers.push_back(_buffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& _buffer)
	{
		ZT_PROFILE_FUNCTION();

		glBindVertexArray(mRendererID);
		_buffer->Bind();

		mIndexBuffer = _buffer;
	}

}