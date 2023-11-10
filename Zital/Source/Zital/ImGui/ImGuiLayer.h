#pragma once

#include "Zital/Layer.h"
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
		virtual void OnImGuiRender() override;

		void Begin();
		void End();

	private:
		float mTime = 0.f;
	};

}