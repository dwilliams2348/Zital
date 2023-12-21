#include "EditorLayer.h"
#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>


//temp string map 32x32
//W = water tile
//S = sand tile
//G = grass tile
static const uint32_t sMapWidth = 32;
static const char* sMapTiles =  
"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWSSSSSSWWWWWW"
"WWWWWSSSSSSWWWWSSSSSSGGGGSSWWWWW"
"WWWWSSGGGGSSSSSSGGGGGGGGGGSWWWWW"
"WWWWSGGGGGGGGGGGGGGGGGGGGGSSWWWW"
"WWWWSGGGGGGGGGGGGGGGGGGGGGGSSWWW"
"WWWSSGGGGGGGGGGGGGGGGGGGGGGGSWWW"
"WWWSGGGGGGSSGGGGGGGGGGGGGGGGSWWW"
"WWWSGGGGGSWWSSGGGGGGGGGGGGGGSWWW"
"WWWSGGGGGSWWWWSGGGGGGGGGGGGGSWWW"
"WWSSGGGGGGSWWSGGGGGGGGGGGGGSSWWW"
"WWSGGGGGGGGSSGGGGGGGGGGGGGGSWWWW"
"WWSGGGGGGGGGGGGGGGGGGGGGGGGSWWWW"
"WWSSGGGGGGGGGGGGGGGGGGGGGGGSWWWW"
"WWWSSGGGGGGGGGGGGGGGGGGGGGGSWWWW"
"WWWWSGGGGGGGGGGGGGGGGGGGGGSSWWWW"
"WWWWSGGGGGGGGGGGGGGGGGGGGGSWWWWW"
"WWWWSGGGGGGGGGGGGGGGGGGGGGSWWWWW"
"WWWWSGGGGGGGGGGGGGGGGGGGGSSWWWWW"
"WWWWSSGGGGGGGGGGGGGGGGGSSSWWWWWW"
"WWWWWSSSGGGGGGGGGGGGGGSSWWWWWWWW"
"WWWWWWWSSSGGGGGGGGGGGSSWWWWWWWWW"
"WWWWWWWWWSSSSSSSSSSSSSWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW";


namespace Zital
{

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), mCameraController(1280.f / 720.f)
	{

	}

	void EditorLayer::OnAttach()
	{
		ZT_PROFILE_FUNCTION();

		//mShader = Shader::Create("Assets/Shaders/FlatColor.glsl");
		mTexture = Texture2D::Create("Assets/Textures/checkerboard.png");
		//mTextureSheet = Texture2D::Create("TestGame/Textures/rpgTilemap.png");
		//mTextureCampfire = SubTexture2D::CreateFromCoords(mTextureSheet, { 14.f, 22.f }, { 16.f, 16.f });
		//mTextureTree = SubTexture2D::CreateFromCoords(mTextureSheet, { 17.f, 19.f }, { 16.f, 16.f }, { 1.f, 2.f });
		//
		//mMapWidth = sMapWidth;
		//mMapHeight = strlen(sMapTiles) / sMapWidth;
		//
		//mTextureMap['W'] = SubTexture2D::CreateFromCoords(mTextureSheet, { 3.f, 29.f }, { 16.f, 16.f });
		//mTextureMap['S'] = SubTexture2D::CreateFromCoords(mTextureSheet, { 8.f, 30.f }, { 16.f, 16.f });
		//mTextureMap['G'] = SubTexture2D::CreateFromCoords(mTextureSheet, { 5.f, 30.f }, { 16.f, 16.f });

		FramebufferProperties props;
		props.Width = 1280;
		props.Height = 720;

		mFramebuffer = Framebuffer::Create(props);

		mCameraController.SetZoomLevel(8.f);
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate(Timestep _deltaTime)
	{
		ZT_PROFILE_FUNCTION();
		//Update
		if(mViewportFocused)
			mCameraController.OnUpdate(_deltaTime);

		//Render
		Renderer2D::ResetStats();
		{
			ZT_PROFILE_SCOPE("Render prep");
			mFramebuffer->Bind();
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.f });
			RenderCommand::Clear();
		}

#if 1
		{
			ZT_PROFILE_SCOPE("Render draw");

			static float rotation = 0.f;
			rotation += _deltaTime * 90.f;

			Renderer2D::BeginScene(mCameraController.GetCamera());

			Renderer2D::DrawRotatedQuad({ 1.5f, -0.5f }, { 0.8f, 0.8f }, glm::radians(rotation), { 0.8f, 0.2f, 0.3f, 1.f });
			Renderer2D::DrawQuad({ -1.f, 0.f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.f });
			Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.75f, 0.4f }, { 0.3f, 0.2f, 0.8f, 1.f });
			Renderer2D::DrawQuad({ 0.f, 0.f, -0.1f }, { 20.f, 20.f }, mTexture, 10.f);
			Renderer2D::DrawRotatedQuad({ 0.f, 0.f, 0.f }, { 1.f, 1.f }, 45.f, mTexture, 5.f);

			Renderer2D::EndScene();

			Renderer2D::BeginScene(mCameraController.GetCamera());

			for (float y = -5.f; y < 5.f; y += 0.5f)
			{
				for (float x = -5.f; x < 5.f; x += 0.5f)
				{
					glm::vec4 color = { (x + 5.f) / 10.f, 0.5f, (y + 5.f) / 10.f, 0.75f };
					Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
				}
			}

			Renderer2D::EndScene();

		}
#endif

#if 0
		Renderer2D::BeginScene(mCameraController.GetCamera());

		for (uint32_t y = 0; y < mMapHeight; y++)
		{
			for (uint32_t x = 0; x < mMapWidth; x++)
			{
				char tileType = sMapTiles[(y * mMapWidth) + x];
				Ref<SubTexture2D> texture;

				if (mTextureMap.find(tileType) != mTextureMap.end())
					texture = mTextureMap[tileType];
				else
					texture = mTextureCampfire;

				Renderer2D::DrawQuad({ (float)x - (mMapWidth / 2.f), mMapHeight - (float)y - (mMapHeight / 2.f) }, { 1.f, 1.f }, texture);
			}
		}

		//Renderer2D::DrawQuad({ 0.f, 0.f, 0.f }, { 1.f, 1.f }, mTextureCampfire);
		//Renderer2D::DrawQuad({ 1.f, 0.f, 0.f }, { 1.f, 2.f }, mTextureTree);

		Renderer2D::EndScene();

#endif

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

			ImGui::Begin("Settings");

			auto stats = Renderer2D::GetStats();

			ImGui::Text("Renderer2D stats:");
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quad Count: %d", stats.QuadCount);
			ImGui::Text("Vertex Count: %d", stats.GetTotalVertexCount());
			ImGui::Text("Index Count: %d", stats.GetTotalIndexCount());

			ImGui::ColorEdit4("Square Color", glm::value_ptr(mSquareColor));

			ImGui::End();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0.f, 0.f });
			ImGui::Begin("Viewport");

			mViewportFocused = ImGui::IsWindowFocused();
			mViewportHovered = ImGui::IsWindowHovered();
			Application::Get().GetImGuiLayer()->SetBlockEvents(!mViewportFocused || !mViewportHovered);

			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			if (mViewportSize != *((glm::vec2*)&viewportPanelSize))
			{
				mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
				mFramebuffer->Resize((uint32_t)mViewportSize.x, (uint32_t)mViewportSize.y);

				mCameraController.OnResize(mViewportSize.x, mViewportSize.y);
			}
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