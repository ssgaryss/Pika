#include <Pika.h>
#include <memory>

class Sandbox : public Pika::Application
{
public:
	Sandbox() {};
	~Sandbox() {};
};

Pika::Application* Pika::createApplication() {
	return new Sandbox();
}