#include <Zital.h>

class ExampleLayer : public Zital::Layer
{
public:
	ExampleLayer()
		: Layer("Example") 
	{
	}


	void OnUpdate() override
	{
		ZT_INFO("ExampleLayer::Update");
	}

	void OnEvent(Zital::Event& _event) override
	{
		ZT_TRACE("{0}", _event);
	}

};

class Sandbox : public Zital::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Zital::ImGuiLayer());
	}

	~Sandbox()
	{

	}
};

Zital::Application* Zital::CreateApplication()
{
	return new Sandbox();
}