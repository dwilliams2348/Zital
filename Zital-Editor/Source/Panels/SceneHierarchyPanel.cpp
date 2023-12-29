#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>

#include "Zital/Scene/Components.h"

namespace Zital
{

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& _context)
	{
		SetContext(_context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& _context)
	{
		mContext = _context;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		mContext->mRegistry.each([&](auto _entityID)
			{
				Entity entity { _entityID, mContext.get() };

				DrawEntityNode(entity);
			});

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity _entity)
	{
		auto& tag = _entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((mSelectionContext == _entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)_entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			mSelectionContext = _entity;
		}

		if (opened)
		{
			//temp demo
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx((void*)56736784, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();

			ImGui::TreePop();
		}
	}

}