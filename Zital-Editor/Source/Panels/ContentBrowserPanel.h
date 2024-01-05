#pragma once

#include <filesystem>

#include "Zital/Renderer/Texture.h"

namespace Zital
{

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnImGuiRender();

	private:
		std::filesystem::path mCurrentDirectory;

		Ref<Texture2D> mFolderIcon;
		Ref<Texture2D> mFileIcon;
	};

}