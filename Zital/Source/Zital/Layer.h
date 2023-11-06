#pragma once

#include "Zital/Core.h"
#include "Zital/Events/Event.h"

namespace Zital
{

	class ZITAL_API Layer
	{
	public:
		Layer(const std::string& _name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& _event) {}

		inline const std::string& GetName() const { return mDebugName; }

	protected:
		std::string mDebugName;
	};

}