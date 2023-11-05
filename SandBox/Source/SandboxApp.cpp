#include <Zital.h>

class Sandbox : public Zital::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

Zital::Application* Zital::CreateApplication()
{
	return new Sandbox();
}