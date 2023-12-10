#include "Sandbox2D.h"
#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), mCameraController(1280.f / 720.f)
{

}

void Sandbox2D::OnAttach()
{
	mSquareVA = (Zital::VertexArray::Create());

	float squareVertices[3 * 4] =
	{
		-0.5f, -0.5f, 0.f,
		 0.5f, -0.5f, 0.f,
		 0.5f,  0.5f, 0.f,
		-0.5f,  0.5f, 0.f
	};

	Zital::Ref<Zital::VertexBuffer> SquareVB;
	SquareVB = Zital::VertexBuffer::Create(squareVertices, sizeof(squareVertices));

	SquareVB->SetLayout({
		{Zital::ShaderDataType::Float3, "aPosition"} });

	mSquareVA->AddVertexBuffer(SquareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Zital::Ref<Zital::IndexBuffer> squareIB;
	squareIB = Zital::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));

	mSquareVA->SetIndexBuffer(squareIB);

	mShader = Zital::Shader::Create("Assets/Shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Zital::Timestep _deltaTime)
{
	//Update
	mCameraController.OnUpdate(_deltaTime);

	//Render
	Zital::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
	Zital::RenderCommand::Clear();

	Zital::Renderer2D::BeginScene(mCameraController.GetCamera());

	Zital::Renderer2D::DrawQuad({ -1.f, 0.f }, 50.f, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.f });
	Zital::Renderer2D::DrawQuad({ 0.5f, -0.5f }, 0.f, { 0.75f, 0.4f }, { 0.3f, 0.2f, 0.8f, 1.f });

	Zital::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(mSquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Zital::Event& e)
{
	mCameraController.OnEvent(e);
}
