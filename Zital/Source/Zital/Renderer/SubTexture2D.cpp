#include "ZTpch.h"
#include "SubTexture2D.h"

namespace Zital
{

	SubTexture2D::SubTexture2D(const Ref<Texture2D>& _texture, const glm::vec2& _min, const glm::vec2& _max)
		: mTexture(_texture)
	{
		mTexCoords[0] = { _min.x, _min.y };
		mTexCoords[1] = { _max.x, _min.y };
		mTexCoords[2] = { _max.x, _max.y };
		mTexCoords[3] = { _min.x, _max.y };
	}

	Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& _texture, const glm::vec2& _coords, const glm::vec2& _cellSize, const glm::vec2& _spriteSize)
	{
		glm::vec2 min = { (_coords.x * _cellSize.x) / _texture->GetWidth(), (_coords.y * _cellSize.y) / _texture->GetHeight() };
		glm::vec2 max = { ((_coords.x + _spriteSize.x) * _cellSize.x) / _texture->GetWidth(), ((_coords.y + _spriteSize.y) * _cellSize.y) / _texture->GetHeight() };
		
		return CreateRef<SubTexture2D>(_texture, min, max);
	}

}