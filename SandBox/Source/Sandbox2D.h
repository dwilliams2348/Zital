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

	//Temp texture
	Zital::Ref<Zital::Texture2D> mTexture;

	//temp
	Zital::Timestep ts;

	glm::vec4 mSquareColor = { 0.2f, 0.3f, 0.8f , 1.f};

	uint32_t mMapWidth, mMapHeight;
	std::unordered_map<char, Zital::Ref<Zital::SubTexture2D>> mTextureMap;
};