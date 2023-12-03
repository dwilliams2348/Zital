#pragma once

#include "Zital/Renderer/Texture.h"

namespace Zital
{

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& _filepath);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return mWidth; }
		virtual uint32_t GetHeight() const override { return mHeight; }

		virtual void Bind(uint32_t _slot = 0) const override;

	private:
		std::string mFilepath;
		uint32_t mWidth, mHeight;
		uint32_t mRendererID;
	};

}