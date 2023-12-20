#include "Sandbox2D.h"
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


Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), mCameraController(1280.f / 720.f)
{

}

void Sandbox2D::OnAttach()
{
	ZT_PROFILE_FUNCTION();

	//mShader = Zital::Shader::Create("Assets/Shaders/FlatColor.glsl");
	mTexture = Zital::Texture2D::Create("Assets/Textures/checkerboard.png");
	mTextureSheet = Zital::Texture2D::Create("TestGame/Textures/rpgTilemap.png");
	mTextureCampfire = Zital::SubTexture2D::CreateFromCoords(mTextureSheet, { 14.f, 22.f }, { 16.f, 16.f });
	mTextureTree = Zital::SubTexture2D::CreateFromCoords(mTextureSheet, { 17.f, 19.f }, { 16.f, 16.f }, { 1.f, 2.f });

	mMapWidth = sMapWidth;
	mMapHeight = strlen(sMapTiles) / sMapWidth;

	mTextureMap['W'] = Zital::SubTexture2D::CreateFromCoords(mTextureSheet, { 3.f, 29.f }, { 16.f, 16.f });
	mTextureMap['S'] = Zital::SubTexture2D::CreateFromCoords(mTextureSheet, { 8.f, 30.f }, { 16.f, 16.f });
	mTextureMap['G'] = Zital::SubTexture2D::CreateFromCoords(mTextureSheet, { 5.f, 30.f }, { 16.f, 16.f });

	mCameraController.SetZoomLevel(8.f);
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

#if 0
	{
		ZT_PROFILE_SCOPE("Render draw");

		static float rotation = 0.f;
		rotation += _deltaTime * 90.f;

		Zital::Renderer2D::BeginScene(mCameraController.GetCamera());

		Zital::Renderer2D::DrawRotatedQuad({ 1.5f, -0.5f }, { 0.8f, 0.8f }, glm::radians(rotation), { 0.8f, 0.2f, 0.3f, 1.f });
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
#endif

#if 0
	Zital::Renderer2D::BeginScene(mCameraController.GetCamera());

	for (uint32_t y = 0; y < mMapHeight; y++)
	{
		for (uint32_t x = 0; x < mMapWidth; x++)
		{
			char tileType = sMapTiles[(y * mMapWidth) + x];
			Zital::Ref<Zital::SubTexture2D> texture;

			if (mTextureMap.find(tileType) != mTextureMap.end())
				texture = mTextureMap[tileType];
			else
				texture = mTextureCampfire;

			Zital::Renderer2D::DrawQuad({ (float)x - (mMapWidth / 2.f), mMapHeight - (float)y - (mMapHeight / 2.f) }, { 1.f, 1.f }, texture);
		}
	}
	
	//Zital::Renderer2D::DrawQuad({ 0.f, 0.f, 0.f }, { 1.f, 1.f }, mTextureCampfire);
	//Zital::Renderer2D::DrawQuad({ 1.f, 0.f, 0.f }, { 1.f, 2.f }, mTextureTree);
	
	Zital::Renderer2D::EndScene();

#endif
}

void Sandbox2D::OnImGuiRender()
{
	ZT_PROFILE_FUNCTION();

	//example docking space using imgui 
	static bool dockspaceOpen = true;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
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

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
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
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.

			if (ImGui::MenuItem("Close")) Zital::Application::Get().Close();
            ImGui::Separator();

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

	ImGui::Begin("Settings");

	auto stats = Zital::Renderer2D::GetStats();

	ImGui::Text("Renderer2D stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quad Count: %d", stats.QuadCount);
	ImGui::Text("Vertex Count: %d", stats.GetTotalVertexCount());
	ImGui::Text("Index Count: %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(mSquareColor));

	ImGui::Image((void*)mTexture->GetRendererID(), ImVec2{ 64.f, 64.f });

	ImGui::End();

    ImGui::End();
}

void Sandbox2D::OnEvent(Zital::Event& e)
{
	mCameraController.OnEvent(e);
}
