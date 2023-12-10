#pragma once

#include "Zital.h"

class Sandbox2D : public Zital::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate(Zital::Timestep _deltaTime) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Zital::Event& e) override;

private:
	Zital::OrthographicCameraController mCameraController;

	//Temporary, will delete, here for testing purpose
	Zital::Ref<Zital::VertexArray> mSquareVA;
	Zital::Ref<Zital::Shader> mShader;

	glm::vec4 mSquareColor = { 0.2f, 0.3f, 0.8f , 1.f};
};