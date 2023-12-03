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

		ZT_CORE_INFO("Image channels: {0}", channels);

		glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
		glTextureStorage2D(mRendererID, 1, GL_RGB8, mWidth, mHeight);

		glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(mRendererID, 0, 0, 0, mWidth, mHeight, GL_RGB, GL_UNSIGNED_BYTE, data);

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