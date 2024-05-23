#pragma once

#include "Pika/Core/Layer.h"
#include "Pika/Events/Event.h"
#include "Pika/Events/ApplicationEvent.h"
#include "Pika/Events/KeyboardEvent.h"
#include "Pika/Events/MouseEvent.h"


namespace Pika {

	class PIKA_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer(const std::string& vLayerName = "ImGui");
		~ImGuiLayer();
		void onAttach() override;
		void onDetach() override;
		void onUpdate() override;
		void onEvent(Event& vEvent) override;
	private:
		bool onMouseButtonPressedEvent(MouseButtonPressedEvent& vEvent);
		bool onMouseButtonRleasedEvent(MouseButtonRleasedEvent& vEvent);
		bool onMouseMovedEvent(MouseMovedEvent& vEvent);
		bool onMouseScrolledEvent(MouseScrolledEvent& vEvent);
		bool onKeyPressedEvent(KeyPressedEvent& vEvent);
		bool onKeyReleasedEvent(KeyReleasedEvent& vEvent);
		bool onKeyTypedEvent(KeyTypedEvent& vEvent);
		bool onWindowResizeEvent(WindowResizeEvent& vEvent);
	private:
		float m_Time = 0.0f;
	};

}

