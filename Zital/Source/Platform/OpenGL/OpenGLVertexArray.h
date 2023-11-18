#pragma once

#include "Zital/Renderer/VertexArray.h"

namespace Zital
{

	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& _buffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& _buffer) override;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return mVertexBuffers; }
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return mIndexBuffer; }

	private:
		uint32_t mRendererID;
		std::vector<std::shared_ptr<VertexBuffer>> mVertexBuffers;
		std::shared_ptr<IndexBuffer> mIndexBuffer;
	};

}