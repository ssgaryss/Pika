#include <Pika.h>
//---------------EntryPoint---------------
#include <Pika/Core/EntryPoint.h>
//----------------------------------------
#include "Sandbox2D.h"
#include "ExampleLayer.h"

class Sandbox : public Pika::Application
{
public:
	Sandbox(const Pika::ApplicationSpecification& vApplicationSpecification)
		:Application{ vApplicationSpecification }
	{
		PK_PROFILE_FUNCTION();

		//pushLayer(new ExampleLayer());
		pushLayer(new Sandbox2D());
	};
	~Sandbox() { PK_PROFILE_FUNCTION(); }
};

Pika::Application* Pika::createApplication() {
	PK_PROFILE_FUNCTION();
	Pika::ApplicationSpecification Specification;
	Specification.m_AppName = "Sandbox2D";
	return new Sandbox(Specification);
}