#include "EditorLayer.h"
#include <imgui/imgui.h>

#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

#include "Zital/Scene/SceneSerializer.h"

#include "Zital/Utils/PlatformUtils.h"

#include "ImGuizmo.h"

#include "Zital/Math/Math.h"

namespace Zital
{

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), mCameraController(1280.f / 720.f)
	{

	}

	void EditorLayer::OnAttach()
	{
		ZT_PROFILE_FUNCTION();

		mTexture = Texture2D::Create("Assets/Textures/checkerboard.png");

		FramebufferProperties props;
		props.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		props.Width = 1280;
		props.Height = 720;

		mFramebuffer = Framebuffer::Create(props);

		mActiveScene = CreateRef<Scene>();

		mEditorCamera = EditorCamera(45.f, 1.778f, 0.1f, 1000.f);

#if 0
		mSquareEntity = mActiveScene->CreateEntity("Square");
		mSquareEntity.AddComponent<TransformComponent>(glm::vec3{ 0.f, 0.f, 0.f });
		mSquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.f, 0.f, 1.f, 1.f });

		mCameraEntity = mActiveScene->CreateEntity("Camera Entity");
		mCameraEntity.AddComponent<TransformComponent>(glm::vec3{ 0.f, 0.f, 0.f });
		mCameraEntity.AddComponent<CameraComponent>();

		//mCameraController.SetZoomLevel(4.f);

		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate()
			{
				
			}

			void OnDestroy()
			{

			}

			void OnUpdate(Timestep _deltaTime)
			{
				auto& translation = GetComponent<TransformComponent>().Translation;
				float speed = 5.f;

				if (Input::IsKeyPressed(KeyCode::A))
					translation.x -= speed * _deltaTime;
				if (Input::IsKeyPressed(KeyCode::D))
					translation.x += speed * _deltaTime;
				if (Input::IsKeyPressed(KeyCode::W))
					translation.y += speed * _deltaTime;
				if (Input::IsKeyPressed(KeyCode::S))
					translation.y -= speed * _deltaTime;
			}
		};

		mCameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif

		mSceneHierarchyPanel.SetContext(mActiveScene);
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate(Timestep _deltaTime)
	{
		ZT_PROFILE_FUNCTION();

		//Resize
		if (FramebufferProperties prop = mFramebuffer->GetProperties(); 
			mViewportSize.x > 0.f && mViewportSize.y > 0.f && //zero sized is invalid
			(prop.Width != mViewportSize.x || prop.Height != mViewportSize.y))
		{
			mFramebuffer->Resize((uint32_t)mViewportSize.x, (uint32_t)mViewportSize.y);
			mCameraController.OnResize(mViewportSize.x, mViewportSize.y);
			mEditorCamera.SetViewportSize(mViewportSize.x, mViewportSize.y);
			mActiveScene->OnViewportResize((uint32_t)mViewportSize.x, (uint32_t)mViewportSize.y);
		}

		//Update
		//if (mViewportFocused)
			mEditorCamera.OnUpdate(_deltaTime);

		//Render
		Renderer2D::ResetStats();
		mFramebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		RenderCommand::Clear();

		//clear entityID attachment to -1
		mFramebuffer->ClearAttachment(1, -1);

		//update scene
		mActiveScene->OnUpdateEditor(_deltaTime, mEditorCamera);

		auto [mx, my] = ImGui::GetMousePos();
		mx -= mViewportBounds[0].x;
		my -= mViewportBounds[0].y;
		my = mViewportSize.y - my;

		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)mViewportSize.x && mouseY < (int)mViewportSize.y)
		{
			int pixelData = mFramebuffer->ReadPixel(1, mouseX, mouseY);
			mHoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, mActiveScene.get());
		}

		mFramebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		ZT_PROFILE_FUNCTION();

		static bool dockingEnabled = true;
		if (dockingEnabled)
		{
			//example docking space using imgui 
			static bool dockspaceOpen = true;
			static bool opt_fullscreen = true;
			static bool opt_padding = false;
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
			if (opt_fullscreen)
			{
				const ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImGui::SetNextWindowPos(viewport->WorkPos);
				ImGui::SetNextWindowSize(viewport->WorkSize);
				ImGui::SetNextWindowViewport(viewport->ID);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
				ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
				window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
			}
			else
			{
				dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
			}

			if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				window_flags |= ImGuiWindowFlags_NoBackground;

			if (!opt_padding)
				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
			if (!opt_padding)
				ImGui::PopStyleVar();

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);

			// Submit the DockSpace
			ImGuiIO& io = ImGui::GetIO();
			ImGuiStyle& style = ImGui::GetStyle();
			float minWinSize = style.WindowMinSize.x;
			style.WindowMinSize.x = 370.f;
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			style.WindowMinSize.x = minWinSize;

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("New", "Ctrl+N"))
						NewScene();

					if (ImGui::MenuItem("Open...", "Ctrl+O"))
						OpenScene();

					if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
						SaveSceneAs();

					ImGui::Separator();

					if (ImGui::MenuItem("Close")) Application::Get().Close();

					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			mSceneHierarchyPanel.OnImGuiRender();

			ImGui::Begin("Stats");

			const char* name = mHoveredEntity ? mHoveredEntity.GetComponent<TagComponent>().Tag.c_str() : "none";
			ImGui::Text("Hovered Entity: %s", name);

			auto stats = Renderer2D::GetStats();

			ImGui::Text("Renderer2D stats:");
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quad Count: %d", stats.QuadCount);
			ImGui::Text("Vertex Count: %d", stats.GetTotalVertexCount());
			ImGui::Text("Index Count: %d", stats.GetTotalIndexCount());

			ImGui::End();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.f, 0.f });
			ImGui::Begin("Viewport");
			auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
			auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
			auto viewportOffset = ImGui::GetWindowPos();
			mViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
			mViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

			mViewportFocused = ImGui::IsWindowFocused();
			mViewportHovered = ImGui::IsWindowHovered();
			Application::Get().GetImGuiLayer()->SetBlockEvents(!mViewportFocused && !mViewportHovered);

			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

			mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			uint32_t textureID = mFramebuffer->GetColorAttachmentRendererID();
			ImGui::Image((void*)textureID, ImVec2{ mViewportSize.x, mViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			//gizmos
			Entity selectedEntity = mSceneHierarchyPanel.GetSelectedEntity();
			if (selectedEntity && mGizmoType != -1 && selectedEntity.HasComponent<TransformComponent>())
			{
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();

				ImGuizmo::SetRect(mViewportBounds[0].x, mViewportBounds[0].y, mViewportBounds[1].x - mViewportBounds[0].x, mViewportBounds[1].y - mViewportBounds[0].y);

				//camera
				//runtime camera from entity
				//auto cameraEntity = mActiveScene->GetPrimaryCameraEntity();
				//const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
				//const glm::mat4& cameraProjection = camera.GetProjection();
				//glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

				//editor camera
				const glm::mat4& cameraProjection = mEditorCamera.GetProjection();
				glm::mat4 cameraView = mEditorCamera.GetViewMatrix();

				//entity transform
				auto& transformComponent = selectedEntity.GetComponent<TransformComponent>();
				glm::mat4 transform = transformComponent.GetTransform();

				//snapping this is currently only local snapping
				bool snap = Input::IsKeyPressed(Key::LeftControl);
				float snapValue = 0.5f; //snap to 0.5f units for translation/scale
				//snap to 45.f for rotation gizmo
				if (mGizmoType == ImGuizmo::OPERATION::ROTATE)
					snapValue = 45.f;

				float snapValues[3] = { snapValue, snapValue, snapValue };

				ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
					(ImGuizmo::OPERATION)mGizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
					nullptr, snap ? snapValues : nullptr);

				if (ImGuizmo::IsUsing())
				{
					glm::vec3 translation, rotation, scale;
					Math::DecomposeTransform(transform, translation, rotation, scale);

					glm::vec3 deltaRotation = rotation - transformComponent.Rotation;
					transformComponent.Translation = translation;
					transformComponent.Rotation += deltaRotation;
					transformComponent.Scale = scale;
				}
			}

			ImGui::End();
			ImGui::PopStyleVar();

			ImGui::End();
		}
	}

	void EditorLayer::OnEvent(Event& e)
	{
		mCameraController.OnEvent(e);
		mEditorCamera.OnEvent(e);

		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<KeyPressedEvent>(ZT_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(ZT_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		//Shortcuts
		if (e.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
		switch (e.GetKeyCode())
		{
			case Key::N:
				if (control && !ImGuizmo::IsUsing())
					NewScene();

				break;
			case Key::O:
				if (control && !ImGuizmo::IsUsing())
					OpenScene();

				break;
			case Key::S:
				if (control && shift && !ImGuizmo::IsUsing())
					SaveSceneAs();

				break;

				//gizmo shortcuts
			case Key::Q:
				if (!ImGuizmo::IsUsing())
					mGizmoType = -1;
				break;
			case Key::W:
				if (!ImGuizmo::IsUsing())
					mGizmoType = ImGuizmo::OPERATION::TRANSLATE;
				break;
			case Key::E:
				if (!ImGuizmo::IsUsing())
					mGizmoType = ImGuizmo::OPERATION::ROTATE;
				break;
			case Key::R:
				if (!ImGuizmo::IsUsing())
					mGizmoType = ImGuizmo::OPERATION::SCALE;
				break;

		}
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == MouseCode::ButtonLeft)
			if (mViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
				mSceneHierarchyPanel.SetSelectedEntity(mHoveredEntity);

		return false;
	}

	void EditorLayer::NewScene()
	{
		mActiveScene = CreateRef<Scene>();
		mActiveScene->OnViewportResize((uint32_t)mViewportSize.x, (uint32_t)mViewportSize.y);
		mSceneHierarchyPanel.SetContext(mActiveScene);
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Zital Scene (*.zital)\0*.zital\0");
		if (!filepath.empty())
		{
			NewScene();

			SceneSerializer serializer(mActiveScene);
			serializer.Deserialize(filepath);
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Zital Scene (*.zital)\0*.zital\0");
		if (!filepath.empty())
		{
			SceneSerializer serializer(mActiveScene);
			serializer.Serialize(filepath);
		}
	}

}