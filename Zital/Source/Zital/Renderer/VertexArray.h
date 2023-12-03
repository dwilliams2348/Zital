#pragma once

#include <memory>
#include "Zital/Renderer/Buffer.h"

namespace Zital
{

	class VertexArray
	{
	public:
		~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& _buffer) = 0;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& _buffer) = 0;

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0;
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;

		static Ref<VertexArray> Create();
	};

}