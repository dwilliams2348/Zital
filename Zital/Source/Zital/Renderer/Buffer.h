#pragma once


namespace Zital
{

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		static VertexBuffer* Create(float* _vertices, uint32_t _size);
	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetIndexCount() const = 0;

		static IndexBuffer* Create(uint32_t* _indices, uint32_t _count);
	};

}