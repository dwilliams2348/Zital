#pragma once

#include <glm/glm.hpp>

#include "Texture.h"

namespace Zital
{

	class SubTexture2D
	{
	public:
		SubTexture2D(const Ref<Texture2D>& _texture, const glm::vec2& _min, const glm::vec2& _max);

		const Ref<Texture2D> GetTexture() const { return mTexture; }
		const glm::vec2* GetTexCoords() const { return mTexCoords; }

		static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& _texture, const glm::vec2& _coords, const glm::vec2& _cellSize, const glm::vec2& _spriteSize = {1, 1});

	private:
		Ref<Texture2D> mTexture;

		glm::vec2 mTexCoords[4];
	};

}