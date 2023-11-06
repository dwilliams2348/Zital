#pragma once

#include "Zital/Layer.h"


namespace Zital
{

	class ZITAL_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& _event);

	private:
		float mTime = 0.f;
	};

}