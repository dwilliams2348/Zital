#pragma once

#include "Zital.h"

namespace Zital
{

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		void OnUpdate(Timestep _deltaTime) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;

	private:
		OrthographicCameraController mCameraController;

		//Temporary, will delete, here for testing purpose
		Ref<VertexArray> mSquareVA;
		Ref<Shader> mShader;
		Ref<Framebuffer> mFramebuffer;

		//Temp texture
		Ref<Texture2D> mTexture;
		Ref<Texture2D> mTextureSheet;
		Ref<SubTexture2D> mTextureCampfire, mTextureTree;

		//temp
		Timestep ts;

		glm::vec4 mSquareColor = { 0.2f, 0.3f, 0.8f , 1.f };

		uint32_t mMapWidth, mMapHeight;
		std::unordered_map<char, Ref<SubTexture2D>> mTextureMap;
	};

}