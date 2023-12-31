#include "ZTpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"

//#define IMGUI_IMPL_API
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Zital/Core/Application.h"

//Temporary
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Zital
{

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::OnAttach()
	{
		ZT_PROFILE_FUNCTION();

		//Setting up Dear imgui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;	//Enables keyboard controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;	//Enables gamepad controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;		//Enables docking of imgui windows
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;		//Enables multi viewport
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewPortsNoMerge;

		io.Fonts->AddFontFromFileTTF("Assets/Fonts/OpenSans/OpenSans-Bold.ttf", 18.f);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("Assets/Fonts/OpenSans/OpenSans-Regular.ttf", 18.f);

		//Setup Dear ImGui style
		ImGui::StyleColorsDark();
		
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.f;
			style.Colors[ImGuiCol_WindowBg].w = 1.f;
		}

		SetDarkThemeColors();

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		//Setup platform render bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");

		//ImGuiIO& io = ImGui::GetIO();
		//io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		//io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

	}

	void ImGuiLayer::OnDetach()
	{
		ZT_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		if (mBlockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::Begin()
	{
		ZT_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ZT_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		//rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backupCurrentContext);
		}
	}

	void ImGuiLayer::SetDarkThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.08f, 0.11f, 1.f };

		//headers
		colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.18f, 0.21f, 1.f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.28f, 0.31f, 1.f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1255f, 0.151f, 1.f };

		//buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.18f, 0.21f, 1.f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.28f, 0.31f, 1.f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1255f, 0.151f, 1.f };

		//frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.18f, 0.21f, 1.f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.28f, 0.31f, 1.f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1255f, 0.151f, 1.f };

		//tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1255f, 0.151f, 1.f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3555f, 0.381f, 1.f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2555f, 0.281f, 1.f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1255f, 0.151f, 1.f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.18f, 0.21f, 1.f };

		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1255f, 0.151f, 1.f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1255f, 0.151f, 1.f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1255f, 0.151f, 1.f };
	}

}