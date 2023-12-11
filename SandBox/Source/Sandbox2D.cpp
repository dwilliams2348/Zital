#include "Sandbox2D.h"
#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

template<typename Fn>
class Timer
{
public:
	Timer(const char* _name, Fn&& _func)
		: mName(_name), mFunc(_func), mStopped(false)
	{
		mStartingTimepoint = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		if (!mStopped)
			Stop();
	}

	void Stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(mStartingTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		mStopped = true;

		float duration = (end - start) * 0.001f;

		mFunc({ mName, duration });
	}

private:
	const char* mName;
	Fn mFunc;
	std::chrono::time_point<std::chrono::steady_clock> mStartingTimepoint;
	bool mStopped;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) {mProfileResults.push_back(profileResult); })

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), mCameraController(1280.f / 720.f)
{

}

void Sandbox2D::OnAttach()
{
	//mShader = Zital::Shader::Create("Assets/Shaders/FlatColor.glsl");
	mTexture = Zital::Texture2D::Create("Assets/Textures/checkerboard.png");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Zital::Timestep _deltaTime)
{
	PROFILE_SCOPE("Sandbox2D::OnUpdate");

	//Update
	{
		PROFILE_SCOPE("CameraController::OnUpdate");
		mCameraController.OnUpdate(_deltaTime);
	}

	//Render
	{
		PROFILE_SCOPE("Renderer Prep");
		Zital::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		Zital::RenderCommand::Clear();
	}

	{
		PROFILE_SCOPE("Renderer Draw");
		Zital::Renderer2D::BeginScene(mCameraController.GetCamera());

		Zital::Renderer2D::DrawQuad({ -1.f, 0.f }, 50.f, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.f });
		Zital::Renderer2D::DrawQuad({ 0.5f, -0.5f }, 0.f, { 0.75f, 0.4f }, { 0.3f, 0.2f, 0.8f, 1.f });
		Zital::Renderer2D::DrawQuad({ 0.f, 0.f, -0.1f }, 0.f, { 10.f, 10.f }, mTexture);

		Zital::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(mSquareColor));

	for (auto& result : mProfileResults)
	{
		char label[50];
		strcpy(label, "%.3fms ");
		strcat(label, result.Name);

		ImGui::Text(label, result.Time);
	}

	mProfileResults.clear();

	ImGui::End();
}

void Sandbox2D::OnEvent(Zital::Event& e)
{
	mCameraController.OnEvent(e);
}
