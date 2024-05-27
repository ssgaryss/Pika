#include "Pika.h"
#include "imgui/imgui.h"

class ExampleLayer : public Pika::Layer
{
public:
	ExampleLayer() :Layer{ "Example Layer" } {};
	void onUpdate() override{ 
		if (Pika::Input::isKeyPressed(Pika::Key::KeyCode::Tab)) {
			PK_TRACE("Tab is pressed!");
		}
	}
	void onEvent(Pika::Event& vEvent) override {
		//PK_TRACE(vEvent.toString());
		if (vEvent.getEventType() == Pika::EventType::KeyPressed) {
			Pika::KeyPressedEvent& Event = static_cast<Pika::KeyPressedEvent&>(vEvent);
			PK_TRACE("Event Layer:Key {0} is pressed!", (char)Event.getKeyCode());
		}
	}
	void onImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::Text("Hello world!");
		ImGui::End();
	}


};

class Sandbox : public Pika::Application
{
public:
	Sandbox() 
		:Application{}
	{
		pushLayer(new ExampleLayer());
		pushOverlay(new Pika::ImGuiLayer());
	};
	~Sandbox() {};
};

Pika::Application* Pika::createApplication() {
	return new Sandbox();
}