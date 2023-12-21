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
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void SetBlockEvents(bool _block) { mBlockEvents = _block; }

	private:
		bool mBlockEvents = true;
		float mTime = 0.f;
	};

}