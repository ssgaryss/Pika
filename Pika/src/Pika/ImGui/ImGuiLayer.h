#pragma once

#include "Pika/Core/Layer.h"
#include "Pika/Events/Event.h"
#include "Pika/Events/ApplicationEvent.h"
#include "Pika/Events/KeyboardEvent.h"
#include "Pika/Events/MouseEvent.h"


namespace Pika {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer(const std::string& vLayerName = "ImGui");
		~ImGuiLayer();
		void onAttach() override;
		void onDetach() override;
		void onEvent(Event& vEvent) override;
		
		void begin();//begin()---- content ------end()
		void end();

		inline void setBlockEvents(bool vIsBlock) { m_BlockEvents = vIsBlock; }
	public:
		// UI∑Á∏Ò…Ë÷√
		void setDarkThemeColors();
	private:
		bool m_BlockEvents = true; // if false, can not handle event
	};

}

