#pragma once

#include "Zital/Core/Core.h"

namespace Zital
{

	class UniformBuffer
	{
	public:
		virtual ~UniformBuffer() {}

		virtual void SetData(const void* _data, uint32_t _size, uint32_t _offset = 0) = 0;

		static Ref<UniformBuffer> Create(uint32_t _size, uint32_t _binding);
	};

}