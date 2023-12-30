#pragma once

#include "Zital/Core/Core.h"
#include "Zital/Scene/Scene.h"
#include "Zital/Scene/Entity.h"

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

		template<typename T>
		void DrawUIComp(const char* _name, Entity _entity, void(*_func)(Entity));

	private:
		Ref<Scene> mContext;
		Entity mSelectionContext;
	};

	template<typename T>
	inline void SceneHierarchyPanel::DrawUIComp(const char* _name, Entity _entity, void(*_func)(Entity))
	{
		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

		if (_entity.HasComponent<T>())
		{
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			bool opened = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, _name);

			if (_entity.GetComponent<T>().Removeable)
			{
				ImGui::SameLine(ImGui::GetWindowWidth() - 50.f);

				if (ImGui::Button("...", ImVec2{ 27.5f, 20 }))
					ImGui::OpenPopup("ComponentSettings");
			}

			ImGui::PopStyleVar();

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (opened)
			{
				_func(_entity);
				
				ImGui::TreePop();
			}

			if (removeComponent)
				_entity.RemoveComponent<T>();
		}
	}

}