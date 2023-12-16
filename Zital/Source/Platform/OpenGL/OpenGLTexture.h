#pragma once

#include "Zital/Renderer/Texture.h"

#include <glad/glad.h>

namespace Zital
{

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t _width, uint32_t _height);
		OpenGLTexture2D(const std::string& _filepath);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return mWidth; }
		virtual uint32_t GetHeight() const override { return mHeight; }

		virtual void SetData(void* _data, uint32_t _size) override;

		virtual void Bind(uint32_t _slot = 0) const override;

		virtual bool operator==(const Texture& _other) const override { return mRendererID == ((OpenGLTexture2D&)_other).mRendererID; };
	private:
		std::string mFilepath;
		uint32_t mWidth, mHeight;
		uint32_t mRendererID;
		GLenum mInternalFormat, mDataFormat;
	};

}