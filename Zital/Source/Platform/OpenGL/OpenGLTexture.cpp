#include "ZTpch.h"
#include "OpenGLTexture.h"

#include <stb_image.h>

namespace Zital
{
	OpenGLTexture2D::OpenGLTexture2D(uint32_t _width, uint32_t _height)
		: mWidth(_width), mHeight(_height)
	{
		ZT_PROFILE_FUNCTION();

		mInternalFormat = GL_RGBA8;
		mDataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
		glTextureStorage2D(mRendererID, 1, mInternalFormat, mWidth, mHeight);

		glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& _filepath)
		: mFilepath(_filepath)
	{
		ZT_PROFILE_FUNCTION();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			ZT_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");

			data = stbi_load(_filepath.c_str(), &width, &height, &channels, 0);
		}
		ZT_CORE_ASSERT(data, "Failed to load image.");
		mWidth = width;
		mHeight = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		ZT_CORE_ASSERT(internalFormat && dataFormat, "Texture color formatting is invalid.");

		mInternalFormat = internalFormat;
		mDataFormat = dataFormat;

		glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
		glTextureStorage2D(mRendererID, 1, internalFormat, mWidth, mHeight);

		glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(mRendererID, 0, 0, 0, mWidth, mHeight, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		ZT_PROFILE_FUNCTION();

		glDeleteTextures(1, &mRendererID);
	}

	void OpenGLTexture2D::SetData(void* _data, uint32_t _size)
	{
		ZT_PROFILE_FUNCTION();

		uint32_t bytesPerPixel = mDataFormat == GL_RGBA ? 4 : 3;
		ZT_CORE_ASSERT(_size == mWidth * mHeight * bytesPerPixel, "Data must be entire texture.");
		glTextureSubImage2D(mRendererID, 0, 0, 0, mWidth, mHeight, mDataFormat, GL_UNSIGNED_BYTE, _data);
	}

	void OpenGLTexture2D::Bind(uint32_t _slot) const
	{
		ZT_PROFILE_FUNCTION();

		glBindTextureUnit(_slot, mRendererID);
	}

}