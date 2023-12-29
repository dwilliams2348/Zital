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

	private:
		Ref<Scene> mContext;
		Entity mSelectionContext;
	};

}