#pragma once

namespace Zital
{

	class ContentBrowserPanel
	{
	public:
		ContentBrowserPanel();

		void OnImGuiRender();

	private:
		std::filesystem::path mCurrentDirectory;
	};

}