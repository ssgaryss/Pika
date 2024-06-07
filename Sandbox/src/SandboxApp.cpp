#include <Pika.h>
//---------------EntryPoint---------------
#include <Pika/Core/EntryPoint.h>
//----------------------------------------
#include "Sandbox2D.h"
#include "ExampleLayer.h"

class Sandbox : public Pika::Application
{
public:
	Sandbox()
		:Application{}
	{
		//pushLayer(new ExampleLayer());
		pushLayer(new Sandbox2D());
	};
	~Sandbox() {};
};

Pika::Application* Pika::createApplication() {
	return new Sandbox();
}