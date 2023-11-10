#include <Zital.h>

#include "imgui/imgui.h"

class ExampleLayer : public Zital::Layer
{
public:
	ExampleLayer()
		: Layer("Example") 
	{
	}


	void OnUpdate() override
	{
		//ZT_INFO("ExampleLayer::Update");

		if (Zital::Input::IsKeyPressed(ZT_KEY_TAB))
			ZT_TRACE("The tab key was pressed.");
	}

	void OnEvent(Zital::Event& _event) override
	{
		if (_event.GetEventType() == Zital::EEventType::KeyPressed)
		{
			Zital::KeyPressedEvent& e = (Zital::KeyPressedEvent&)_event;
			ZT_TRACE("{0}", (char)e.GetKeyCode());
		}
	}

	virtual void OnImGuiRender()
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello world");
		ImGui::End();
	}

};

class Sandbox : public Zital::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{

	}
};

Zital::Application* Zital::CreateApplication()
{
	return new Sandbox();
}