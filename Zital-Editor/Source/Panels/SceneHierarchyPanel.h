#pragma once

#include "Zital/Core/Core.h"
#include "Zital/Scene/Scene.h"
#include "Zital/Scene/Entity.h"

#include <imgui/imgui_internal.h>

namespace Zital
{

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& _context);

		void SetContext(const Ref<Scene>& _context);

		void OnImGuiRender();

	private:
		void DrawEntityNode(Entity _entity);
		void DrawComponents(Entity _entity);

		template<typename T, typename Func>
		void DrawComponent(const char* _name, Entity _entity, Func _func);

	private:
		Ref<Scene> mContext;
		Entity mSelectionContext;
	};

	template<typename T, typename Func>
	inline void SceneHierarchyPanel::DrawComponent(const char* _name, Entity _entity, Func _func)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap
			| ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_FramePadding;

		if (_entity.HasComponent<T>())
		{
			auto& component = _entity.GetComponent<T>();
			ImVec2 contentRegionAvailiable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
			ImGui::Separator();
			bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, _name);
			ImGui::PopStyleVar();

			ImGui::SameLine(contentRegionAvailiable.x - lineHeight * 0.5f);
			if (ImGui::Button("...", ImVec2{ lineHeight, lineHeight }))
				ImGui::OpenPopup("ComponentSettings");

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if(component.Removeable)
					if (ImGui::MenuItem("Remove Component"))
						removeComponent = true;

				ImGui::EndPopup();
			}

			if (opened)
			{
				_func(component);
				
				ImGui::TreePop();
			}

			if (removeComponent)
				_entity.RemoveComponent<T>();
		}
	}

}