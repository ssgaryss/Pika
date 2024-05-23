#include "Pika.h"

class ExampleLayer : public Pika::Layer
{
public:
	ExampleLayer() :Layer{ "Example Layer" } {};
	void onUpdate() { PK_INFO("Example Layer update!"); }
	void onEvent(const Pika::Event& vEvent) { PK_TRACE(vEvent.toString()); }
};

class Sandbox : public Pika::Application
{
public:
	Sandbox() :Application{}
	{
		pushLayer(new ExampleLayer());
		pushOverlay(new Pika::ImGuiLayer());
	};
	~Sandbox() {};
};

Pika::Application* Pika::createApplication() {
	return new Sandbox();
}