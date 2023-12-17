#include "Sandbox2D.h"
#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), mCameraController(1280.f / 720.f)
{

}

void Sandbox2D::OnAttach()
{
	ZT_PROFILE_FUNCTION();

	//mShader = Zital::Shader::Create("Assets/Shaders/FlatColor.glsl");
	mTexture = Zital::Texture2D::Create("Assets/Textures/checkerboard.png");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Zital::Timestep _deltaTime)
{
	ZT_PROFILE_FUNCTION();
	//Update
	mCameraController.OnUpdate(_deltaTime);

	//Render
	Zital::Renderer2D::ResetStats();
	{
		ZT_PROFILE_SCOPE("Render prep");
		Zital::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		Zital::RenderCommand::Clear();
	}

	{
		ZT_PROFILE_SCOPE("Render draw");

		static float rotation = 0.f;
		rotation += _deltaTime * 90.f;

		Zital::Renderer2D::BeginScene(mCameraController.GetCamera());

		Zital::Renderer2D::DrawRotatedQuad({ 1.5f, -0.5f }, { 0.8f, 0.8f }, rotation, { 0.8f, 0.2f, 0.3f, 1.f });
		Zital::Renderer2D::DrawQuad({ -1.f, 0.f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.f });
		Zital::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.75f, 0.4f }, { 0.3f, 0.2f, 0.8f, 1.f });
		Zital::Renderer2D::DrawQuad({ 0.f, 0.f, -0.1f }, { 20.f, 20.f }, mTexture, 10.f);
		Zital::Renderer2D::DrawRotatedQuad({ 0.f, 0.f, 0.f }, { 1.f, 1.f }, 45.f, mTexture, 5.f);

		Zital::Renderer2D::EndScene();

		Zital::Renderer2D::BeginScene(mCameraController.GetCamera());

		for (float y = -5.f; y < 5.f; y += 0.1f)
		{
			for (float x = -5.f; x < 5.f; x += 0.1f)
			{
				glm::vec4 color = { (x + 5.f) / 10.f, 0.5f, (y + 5.f) / 10.f, 0.75f };
				Zital::Renderer2D::DrawQuad({ x, y }, { 0.09f, 0.09f }, color);
			}
		}

		Zital::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	ZT_PROFILE_FUNCTION();

	ImGui::Begin("Settings");

	auto stats = Zital::Renderer2D::GetStats();

	ImGui::Text("Renderer2D stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quad Count: %d", stats.QuadCount);
	ImGui::Text("Vertex Count: %d", stats.GetTotalVertexCount());
	ImGui::Text("Index Count: %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(mSquareColor));

	ImGui::End();
}

void Sandbox2D::OnEvent(Zital::Event& e)
{
	mCameraController.OnEvent(e);
}
