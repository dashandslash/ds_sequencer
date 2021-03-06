#include "ui/ui.h"

#include "imgui/imgui.h"

namespace
{
	void settings()
	{
		ImGui::Begin("Settings");

		auto stats = bgfx::getStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats->numDraw);

		ImGui::End();
	}

	void viewport()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		//m_ViewportFocused = ImGui::IsWindowFocused();
		//m_ViewportHovered = ImGui::IsWindowHovered();
		//Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		//ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		//m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		//uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		//ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void dockSpace()
	{
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", NULL, window_flags);
		ImGui::PopStyleVar();

		ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		
		ImGui::End();
	}
}

namespace ui
{
	void init()
	{
		imguiCreate();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		io.IniFilename = "editorconfig.ini";

		auto style = &ImGui::GetStyle();
		style->WindowRounding = 5.3f;
		style->GrabRounding = style->FrameRounding = 2.3f;
		style->ScrollbarRounding = 5.0f;
		style->FrameBorderSize = 1.0f;
		style->ItemSpacing.y = 6.5f;

		style->Colors[ImGuiCol_Text] = { 0.73333335f, 0.73333335f, 0.73333335f, 1.00f };
		style->Colors[ImGuiCol_TextDisabled] = { 0.34509805f, 0.34509805f, 0.34509805f, 1.00f };
		style->Colors[ImGuiCol_WindowBg] = { 0.23529413f, 0.24705884f, 0.25490198f, 0.94f };
		style->Colors[ImGuiCol_ChildBg] = { 0.23529413f, 0.24705884f, 0.25490198f, 0.00f };
		style->Colors[ImGuiCol_PopupBg] = { 0.23529413f, 0.24705884f, 0.25490198f, 0.94f };
		style->Colors[ImGuiCol_Border] = { 0.33333334f, 0.33333334f, 0.33333334f, 0.50f };
		style->Colors[ImGuiCol_BorderShadow] = { 0.15686275f, 0.15686275f, 0.15686275f, 0.00f };
		style->Colors[ImGuiCol_FrameBg] = { 0.16862746f, 0.16862746f, 0.16862746f, 0.54f };
		style->Colors[ImGuiCol_FrameBgHovered] = { 0.453125f, 0.67578125f, 0.99609375f, 0.67f };
		style->Colors[ImGuiCol_FrameBgActive] = { 0.47058827f, 0.47058827f, 0.47058827f, 0.67f };
		style->Colors[ImGuiCol_TitleBg] = { 0.04f, 0.04f, 0.04f, 1.00f };
		style->Colors[ImGuiCol_TitleBgCollapsed] = { 0.16f, 0.29f, 0.48f, 1.00f };
		style->Colors[ImGuiCol_TitleBgActive] = { 0.00f, 0.00f, 0.00f, 0.51f };
		style->Colors[ImGuiCol_MenuBarBg] = { 0.27058825f, 0.28627452f, 0.2901961f, 0.80f };
		style->Colors[ImGuiCol_ScrollbarBg] = { 0.27058825f, 0.28627452f, 0.2901961f, 0.60f };
		style->Colors[ImGuiCol_ScrollbarGrab] = { 0.21960786f, 0.30980393f, 0.41960788f, 0.51f };
		style->Colors[ImGuiCol_ScrollbarGrabHovered] = { 0.21960786f, 0.30980393f, 0.41960788f, 1.00f };
		style->Colors[ImGuiCol_ScrollbarGrabActive] = { 0.13725491f, 0.19215688f, 0.2627451f, 0.91f };
		// style->Colors[ImGuiCol_ComboBg]               = {0.1f, 0.1f, 0.1f, 0.99f};
		style->Colors[ImGuiCol_CheckMark] = { 0.90f, 0.90f, 0.90f, 0.83f };
		style->Colors[ImGuiCol_SliderGrab] = { 0.70f, 0.70f, 0.70f, 0.62f };
		style->Colors[ImGuiCol_SliderGrabActive] = { 0.30f, 0.30f, 0.30f, 0.84f };
		style->Colors[ImGuiCol_Button] = { 0.33333334f, 0.3529412f, 0.36078432f, 0.49f };
		style->Colors[ImGuiCol_ButtonHovered] = { 0.21960786f, 0.30980393f, 0.41960788f, 1.00f };
		style->Colors[ImGuiCol_ButtonActive] = { 0.13725491f, 0.19215688f, 0.2627451f, 1.00f };
		style->Colors[ImGuiCol_Header] = { 0.33333334f, 0.3529412f, 0.36078432f, 0.53f };
		style->Colors[ImGuiCol_HeaderHovered] = { 0.453125f, 0.67578125f, 0.99609375f, 0.67f };
		style->Colors[ImGuiCol_HeaderActive] = { 0.47058827f, 0.47058827f, 0.47058827f, 0.67f };
		style->Colors[ImGuiCol_Separator] = { 0.31640625f, 0.31640625f, 0.31640625f, 1.00f };
		style->Colors[ImGuiCol_SeparatorHovered] = { 0.31640625f, 0.31640625f, 0.31640625f, 1.00f };
		style->Colors[ImGuiCol_SeparatorActive] = { 0.31640625f, 0.31640625f, 0.31640625f, 1.00f };
		style->Colors[ImGuiCol_ResizeGrip] = { 1.00f, 1.00f, 1.00f, 0.85f };
		style->Colors[ImGuiCol_ResizeGripHovered] = { 1.00f, 1.00f, 1.00f, 0.60f };
		style->Colors[ImGuiCol_ResizeGripActive] = { 1.00f, 1.00f, 1.00f, 0.90f };
		style->Colors[ImGuiCol_PlotLines] = { 0.61f, 0.61f, 0.61f, 1.00f };
		style->Colors[ImGuiCol_PlotLinesHovered] = { 1.00f, 0.43f, 0.35f, 1.00f };
		style->Colors[ImGuiCol_PlotHistogram] = { 0.90f, 0.70f, 0.00f, 1.00f };
		style->Colors[ImGuiCol_PlotHistogramHovered] = { 1.00f, 0.60f, 0.00f, 1.00f };
		style->Colors[ImGuiCol_TextSelectedBg] = { 0.18431373f, 0.39607847f, 0.79215693f, 0.90f };
	}

	void render(const uint32_t& width, const uint32_t& height, const entry::MouseState& mouse)
	{
		imguiBeginFrame(mouse.m_mx
			, mouse.m_my
			, (mouse.m_buttons[entry::MouseButton::Left] ? IMGUI_MBUT_LEFT : 0)
			| (mouse.m_buttons[entry::MouseButton::Right] ? IMGUI_MBUT_RIGHT : 0)
			| (mouse.m_buttons[entry::MouseButton::Middle] ? IMGUI_MBUT_MIDDLE : 0)
			, mouse.m_mz
			, uint16_t(width)
			, uint16_t(height)
		);

		dockSpace();
		settings();
		viewport();

		imguiEndFrame();

	}
}
