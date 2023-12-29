#include "EditorLayer.h"
#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

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
		props.Width = 1280;
		props.Height = 720;

		mFramebuffer = Framebuffer::Create(props);

		mActiveScene = CreateRef<Scene>();

		mSquareEntity = mActiveScene->CreateEntity("Square");
		mSquareEntity.AddComponent<TransformComponent>(glm::mat4(1.f));
		mSquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4{ 0.f, 0.f, 1.f, 1.f });

		mCameraEntity = mActiveScene->CreateEntity("Camera Entity");
		mCameraEntity.AddComponent<TransformComponent>(glm::mat4(1.f));
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
				auto& transform = GetComponent<TransformComponent>().Transform;
				float speed = 5.f;

				if (Input::IsKeyPressed(KeyCode::A))
					transform[3][0] -= speed * _deltaTime;
				if (Input::IsKeyPressed(KeyCode::D))
					transform[3][0] += speed * _deltaTime;
				if (Input::IsKeyPressed(KeyCode::W))
					transform[3][1] += speed * _deltaTime;
				if (Input::IsKeyPressed(KeyCode::S))
					transform[3][1] -= speed * _deltaTime;
			}
		};

		mCameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

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

			mActiveScene->OnViewportResize((uint32_t)mViewportSize.x, (uint32_t)mViewportSize.y);
		}

		//Update
		if(mViewportFocused)
			mCameraController.OnUpdate(_deltaTime);

		//Render
		Renderer2D::ResetStats();
		mFramebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
		RenderCommand::Clear();

		//update scene
		mActiveScene->OnUpdate(_deltaTime);

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
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Close")) Application::Get().Close();
					ImGui::Separator();

					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			mSceneHierarchyPanel.OnImGuiRender();

			ImGui::Begin("Settings");

			auto stats = Renderer2D::GetStats();

			ImGui::Text("Renderer2D stats:");
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quad Count: %d", stats.QuadCount);
			ImGui::Text("Vertex Count: %d", stats.GetTotalVertexCount());
			ImGui::Text("Index Count: %d", stats.GetTotalIndexCount());

			auto& color = mSquareEntity.GetComponent<SpriteRendererComponent>().Color;
			ImGui::ColorEdit4("Square Color", glm::value_ptr(color));

			ImGui::DragFloat3("Camera Transform", glm::value_ptr(mCameraEntity.GetComponent<TransformComponent>().Transform[3]));

			ImGui::End();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.f, 0.f });
			ImGui::Begin("Viewport");

			mViewportFocused = ImGui::IsWindowFocused();
			mViewportHovered = ImGui::IsWindowHovered();
			Application::Get().GetImGuiLayer()->SetBlockEvents(!mViewportFocused || !mViewportHovered);

			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

			mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			uint32_t textureID = mFramebuffer->GetColorAttachmentRendererID();
			ImGui::Image((void*)textureID, ImVec2{ mViewportSize.x, mViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

			ImGui::End();
			ImGui::PopStyleVar();

			ImGui::End();
		}
	}

	void EditorLayer::OnEvent(Event& e)
	{
		mCameraController.OnEvent(e);
	}

}