#pragma once

#include "Pika/Events/Event.h"

namespace Pika {

	class PIKA_API Layer
	{
	public:
		Layer(const std::string& vLayerName = "Layer");
		virtual ~Layer() = default;
		virtual void onAttach() {};
		virtual void onDetach() {};
		virtual void onUpdate() {};
		virtual void onEvent(Event& vEvent) {};
		inline const std::string& getName() const { return m_DebugName; }
	protected:
		std::string m_DebugName; //layer name only used for Debug
	};
}

