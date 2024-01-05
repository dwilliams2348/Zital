#pragma once

#include "Zital.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"

#include "Zital/Renderer/EditorCamera.h"

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
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& _path);
		void SaveSceneAs();

	private:
		OrthographicCameraController mCameraController;

		//Temporary, will delete, here for testing purpose
		Ref<VertexArray> mSquareVA;
		Ref<Shader> mShader;
		Ref<Framebuffer> mFramebuffer;

		//Scene
		Ref<Scene> mActiveScene;
		Entity mSquareEntity;
		Entity mCameraEntity;
		Entity mHoveredEntity;

		EditorCamera mEditorCamera;

		//Temp texture
		Ref<Texture2D> mTexture;
		Ref<Texture2D> mTextureSheet;
		Ref<SubTexture2D> mTextureCampfire, mTextureTree;

		bool mViewportFocused = false, mViewportHovered = false;
		glm::vec2 mViewportSize = { 0.f, 0.f };
		glm::vec2 mViewportBounds[2];

		glm::vec4 mSquareColor = { 0.2f, 0.3f, 0.8f , 1.f };

		int mGizmoType = -1;

		//panels
		SceneHierarchyPanel mSceneHierarchyPanel;
		ContentBrowserPanel mContentBrowserPanel;
	};

}