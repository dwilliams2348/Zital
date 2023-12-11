#pragma once

#include "Zital/Core/Layer.h"
#include "Zital/Events/KeyEvent.h"
#include "Zital/Events/MouseEvent.h"
#include "Zital/Events/ApplicationEvent.h"


namespace Zital
{

	class ZITAL_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void Begin();
		void End();

	private:
		float mTime = 0.f;
	};

}