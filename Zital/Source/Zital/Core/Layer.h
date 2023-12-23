#pragma once

#include "Zital/Core/Core.h"
#include "Zital/Core/TimeStep.h"
#include "Zital/Events/Event.h"

namespace Zital
{

	class Layer
	{
	public:
		Layer(const std::string& _name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep _deltaTime) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& _event) {}

		inline const std::string& GetName() const { return mDebugName; }

	protected:
		std::string mDebugName;
	};

}