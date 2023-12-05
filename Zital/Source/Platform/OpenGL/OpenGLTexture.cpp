#include "ZTpch.h"
#include "OpenGLTexture.h"

#include <stb_image.h>

#include <glad/glad.h>

namespace Zital
{

	OpenGLTexture2D::OpenGLTexture2D(const std::string& _filepath)
		: mFilepath(_filepath)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(_filepath.c_str(), &width, &height, &channels, 0);
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

		glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
		glTextureStorage2D(mRendererID, 1, internalFormat, mWidth, mHeight);

		glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(mRendererID, 0, 0, 0, mWidth, mHeight, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &mRendererID);
	}

	void OpenGLTexture2D::Bind(uint32_t _slot) const
	{
		glBindTextureUnit(_slot, mRendererID);
	}

}