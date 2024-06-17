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
		PK_PROFILE_FUNCTION();

		//pushLayer(new ExampleLayer());
		pushLayer(new Sandbox2D());
	};
	~Sandbox() { PK_PROFILE_FUNCTION(); }
};

Pika::Application* Pika::createApplication() {
	PK_PROFILE_FUNCTION();
	return new Sandbox();
}