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
		if (_entity.HasComponent<T>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(T).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, _name))
			{
				_func(_entity);
				
				ImGui::TreePop();
			}
		}
	}

}