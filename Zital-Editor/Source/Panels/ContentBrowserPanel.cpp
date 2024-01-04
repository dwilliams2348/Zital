#include "ZTpch.h"
#include "ContentBrowserPanel.h"

#include <imgui/imgui.h>

#include <filesystem>

namespace Zital
{
	//will be removed when projects are implemented
	const static std::filesystem::path sAssetsPath = "Assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: mCurrentDirectory(sAssetsPath)
	{
	}

	void ContentBrowserPanel::OnImGuiRender()
	{
		ImGui::Begin("Content Browser");

		if(mCurrentDirectory != std::filesystem::path(sAssetsPath))
		{
			if (ImGui::Button("<="))
			{
				mCurrentDirectory = mCurrentDirectory.parent_path();
			}
		}

		for (auto& directoryEntry : std::filesystem::directory_iterator(mCurrentDirectory))
		{
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(directoryEntry.path(), sAssetsPath);
			std::string filenameString = relativePath.filename().string();
			if(directoryEntry.is_directory())
			{
				if (ImGui::Button(filenameString.c_str()))
				{
					mCurrentDirectory /= path.filename();
				}
			}
			else
			{
				if (ImGui::Button(filenameString.c_str()))
				{
				}
			}
		}

		ImGui::End();
	}

}