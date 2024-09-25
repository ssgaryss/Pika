#include <Pika.h>
//---------------EntryPoint---------------
#include <Pika/Core/EntryPoint.h>
//----------------------------------------
#include "EditorLayer.h"

namespace Pika 
{

	class Pikatisan : public Application
	{
	public:
		Pikatisan(const ApplicationSpecification& vApplicationSpecification)
			:Application{vApplicationSpecification}
		{
			PK_PROFILE_FUNCTION();

			//pushLayer(new ExampleLayer());
			pushLayer(new EditorLayer());
		};
		~Pikatisan() { PK_PROFILE_FUNCTION(); }
	};

	Application* createApplication() {
		PK_PROFILE_FUNCTION();
		ApplicationSpecification Specification;
		Specification.m_AppName = "Pika Engine";
		return new Pikatisan(Specification);
	}

}