#pragma once

#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#define MY_THEME_COLOR_PRIMARY      ImVec4(0.2f, 0.2f, 0.2f, 1.0f)
#define MY_THEME_COLOR_SECONDARY    ImVec4(0.15f, 0.15f, 0.15f, 1.0f)
#define MY_THEME_COLOR_TEXT         ImVec4(0.8f, 0.8f, 0.8f, 1.0f)
#define MY_THEME_COLOR_WINDOW_BG    ImVec4(0.1f, 0.1f, 0.1f, 1.0f)
#define MY_THEME_COLOR_BORDER       ImVec4(0.3f, 0.3f, 0.3f, 0.5f)
#define MY_THEME_COLOR_HEADER       ImVec4(0.15f, 0.15f, 0.15f, 1.0f)
#define MY_THEME_COLOR_COLLAPSED    ImVec4(0.3f, 0.3f, 0.3f, 0.6f)
#define MY_THEME_COLOR_HOVER        ImVec4(0.25f, 0.25f, 0.25f, 1.0f)
void GUIDefaultCongoration()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.WindowPadding = ImVec2(15.0f, 15.0f);
	style.FramePadding = ImVec2(8.0f, 4.0f);
	style.ItemSpacing = ImVec2(8.0f, 6.0f);
	style.ItemInnerSpacing = ImVec2(6.0f, 6.0f);
	style.IndentSpacing = 20.0f;
	style.ScrollbarSize = 15.0f;
	style.GrabMinSize = 10.0f;
	style.WindowBorderSize = 1.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupBorderSize = 1.0f;
	style.FrameBorderSize = 1.0f;
	style.TabBorderSize = 1.0f;
	style.WindowRounding = 4.0f;
	style.ChildRounding = 4.0f;
	style.FrameRounding = 3.0f;
	style.PopupRounding = 4.0f;
	style.ScrollbarRounding = 9.0f;
	style.GrabRounding = 3.0f;
	style.TabRounding = 4.0f;

}

void JonayesTheme()
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = ImGui::GetStyle().Colors;
	style.WindowPadding = ImVec2(5, 5);
	style.WindowRounding = 4.0f;
	style.FramePadding = ImVec2(5, 5);
	style.FrameRounding = 2000.0f;
	style.ItemSpacing = ImVec2(12, 8);
	style.ItemInnerSpacing = ImVec2(8, 6);
	style.IndentSpacing = 25.0f;
	style.ScrollbarSize = 12.0f;
	style.ScrollbarRounding = 100.0f;
	style.GrabMinSize = 5.0f;
	style.GrabRounding = 3.0f;
	colors[ImGuiCol_Text] = ImVec4(0.72 + 0.2, 0.35 + 0.2, 0.76 + 0.2, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.01, 0.01, 0.07, 1);
	colors[ImGuiCol_FrameBg] = ImVec4(0.07, 0.00, 0.06, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.07 + 0.2, 0.00 + 0.2, 0.06 + 0.2, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.07 + 0.1, 0.00 + 0.1, 0.06 + 0.1, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.44, 0.08, 0.71, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.44 + 0.1, 0.08 + 0.1, 0.71 + 0.1, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.07, 0.00, 0.06, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.44, 0.08, 0.71, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.44 + 0.1, 0.08 + 0.1, 0.71 + 0.1, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.44 - 0.1, 0.08 - 0.1, 0.71 - 0.1, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.07, 0.00, 0.06, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.07 + 0.2f, 0.00 + 0.2f, 0.06 + 0.2f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.07, 0.00, 0.06, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(1, 1, 1, 0.29f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.07, 0.00, 0.06, 1.00f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
}

void Cyan_Blue_Theme()
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = ImGui::GetStyle().Colors;
	style.WindowPadding = ImVec2(15, 15);
	style.WindowRounding = 5.0f;
	style.FramePadding = ImVec2(5, 5);
	style.FrameRounding = 4.0f;
	style.ItemSpacing = ImVec2(12, 8);
	style.ItemInnerSpacing = ImVec2(8, 6);
	style.IndentSpacing = 25.0f;
	style.ScrollbarSize = 15.0f;
	style.ScrollbarRounding = 9.0f;
	style.GrabMinSize = 5.0f;
	style.GrabRounding = 3.0f;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.22, 0.25, 0.25, 1);
	colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.56f, 0.83f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.29f, 0.71f, 0.93f, 0.54f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.29f, 0.71f, 0.93f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.56f, 0.83f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.68f, 1.00f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.56f, 0.83f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.00f, 0.56f, 0.83f, 0.54f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.29f, 0.71f, 0.93f, 0.54f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.29f, 0.71f, 0.93f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.00f, 0.56f, 0.83f, 0.52f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.56f, 0.83f, 0.36f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.29f, 0.71f, 0.93f, 0.33f);
	colors[ImGuiCol_Border] = ImVec4(1, 1, 1, 0.29f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);


}

void FutureTheme()
{
	GUIDefaultCongoration();

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.00f, 0.40f, 0.41f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.80f);
	colors[ImGuiCol_Border] = ImVec4(0.00f, 1.00f, 1.00f, 0.65f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.44f, 0.80f, 0.80f, 0.18f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.44f, 0.80f, 0.80f, 0.27f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.44f, 0.81f, 0.86f, 0.66f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.14f, 0.18f, 0.21f, 0.73f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.27f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.22f, 0.29f, 0.30f, 0.71f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.44f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 1.00f, 1.00f, 0.68f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.36f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.76f);
	colors[ImGuiCol_Button] = ImVec4(0.00f, 0.65f, 0.65f, 0.46f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.01f, 1.00f, 1.00f, 0.43f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.62f);
	colors[ImGuiCol_Header] = ImVec4(0.00f, 1.00f, 1.00f, 0.33f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.42f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 1.00f, 1.00f, 0.22f);


}

void LightTheme()
{
	ImVec4* colors = ImGui::GetStyle().Colors;
	GUIDefaultCongoration();
	colors[ImGuiCol_Border] = ImVec4(0, 0, 0, 0.29f);
	colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
}


void HackerTheme()
{
	ImVec4* colors = ImGui::GetStyle().Colors;
	GUIDefaultCongoration();
	colors[ImGuiCol_Border] = ImVec4(0, 1, 0, 0.29f);
	colors[ImGuiCol_Text] = ImVec4(0.00f, 1.00f, 0.00f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);

}

void EyESoRETheme()
{
	ImVec4* colors = ImGui::GetStyle().Colors;
	GUIDefaultCongoration();
	colors[ImGuiCol_Text] = ImVec4(1.00f, 0.20f, 0.80f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.70f, 0.50f, 1.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.80f, 0.00f, 0.40f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.50f, 0.20f, 0.90f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.50f, 0.20f, 0.90f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.60f, 0.90f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.80f, 0.00f, 0.40f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.90f, 0.80f, 0.10f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.80f, 0.00f, 0.40f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.50f, 0.20f, 0.90f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.50f, 0.20f, 0.90f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.80f, 0.00f, 0.40f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.50f, 0.20f, 0.90f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.50f, 0.20f, 0.90f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
}
void DefaultTheme()
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;

	// Modify ImGui style settings
	style.WindowPadding = ImVec2(10, 10);
	style.WindowRounding = 4.0f;
	style.FrameRounding = 4.0f;
	style.GrabRounding = 4.0f;
	style.ScrollbarRounding = 4.0f;
	style.FrameBorderSize = 1.0f;
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);



	colors[ImGuiCol_Text] = MY_THEME_COLOR_TEXT;
	colors[ImGuiCol_TextDisabled] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
	colors[ImGuiCol_WindowBg] = MY_THEME_COLOR_WINDOW_BG;
	colors[ImGuiCol_Border] = MY_THEME_COLOR_BORDER;
	colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	colors[ImGuiCol_FrameBg] = MY_THEME_COLOR_SECONDARY;
	colors[ImGuiCol_FrameBgHovered] = MY_THEME_COLOR_HOVER;
	colors[ImGuiCol_FrameBgActive] = MY_THEME_COLOR_PRIMARY;
	colors[ImGuiCol_TitleBg] = MY_THEME_COLOR_HEADER;
	colors[ImGuiCol_TitleBgActive] = MY_THEME_COLOR_HEADER;
	colors[ImGuiCol_TitleBgCollapsed] = MY_THEME_COLOR_HEADER;
	colors[ImGuiCol_MenuBarBg] = MY_THEME_COLOR_SECONDARY;
	colors[ImGuiCol_ScrollbarBg] = MY_THEME_COLOR_SECONDARY;
	colors[ImGuiCol_ScrollbarGrab] = MY_THEME_COLOR_PRIMARY;
	colors[ImGuiCol_ScrollbarGrabHovered] = MY_THEME_COLOR_HOVER;
	colors[ImGuiCol_ScrollbarGrabActive] = MY_THEME_COLOR_PRIMARY;
	colors[ImGuiCol_CheckMark] = MY_THEME_COLOR_PRIMARY;
	colors[ImGuiCol_SliderGrab] = MY_THEME_COLOR_PRIMARY;
	colors[ImGuiCol_SliderGrabActive] = MY_THEME_COLOR_PRIMARY;
	colors[ImGuiCol_Button] = MY_THEME_COLOR_PRIMARY;
	colors[ImGuiCol_ButtonHovered] = MY_THEME_COLOR_HOVER;
	colors[ImGuiCol_ButtonActive] = MY_THEME_COLOR_PRIMARY;
	colors[ImGuiCol_Header] = MY_THEME_COLOR_PRIMARY;
	colors[ImGuiCol_HeaderHovered] = MY_THEME_COLOR_HOVER;
	colors[ImGuiCol_HeaderActive] = MY_THEME_COLOR_PRIMARY;
	colors[ImGuiCol_ResizeGrip] = MY_THEME_COLOR_PRIMARY;
	colors[ImGuiCol_ResizeGripHovered] = MY_THEME_COLOR_HOVER;
	colors[ImGuiCol_ResizeGripActive] = MY_THEME_COLOR_PRIMARY;
	colors[ImGuiCol_PlotLines] = MY_THEME_COLOR_PRIMARY;
	colors[ImGuiCol_PlotLinesHovered] = MY_THEME_COLOR_PRIMARY;
	colors[ImGuiCol_PlotHistogram] = MY_THEME_COLOR_PRIMARY;
	colors[ImGuiCol_PlotHistogramHovered] = MY_THEME_COLOR_PRIMARY;
	colors[ImGuiCol_TextSelectedBg] = MY_THEME_COLOR_PRIMARY;
	colors[ImGuiCol_DragDropTarget] = MY_THEME_COLOR_PRIMARY;
	colors[ImGuiCol_NavHighlight] = MY_THEME_COLOR_PRIMARY;
	colors[ImGuiCol_NavWindowingHighlight] = MY_THEME_COLOR_PRIMARY;
	colors[ImGuiCol_NavWindowingDimBg] = MY_THEME_COLOR_SECONDARY;
	colors[ImGuiCol_ModalWindowDimBg] = MY_THEME_COLOR_SECONDARY;
	colors[ImGuiCol_Tab] = MY_THEME_COLOR_SECONDARY;
	colors[ImGuiCol_TabHovered] = MY_THEME_COLOR_HOVER;
	colors[ImGuiCol_TabActive] = MY_THEME_COLOR_PRIMARY;
	colors[ImGuiCol_TabUnfocused] = MY_THEME_COLOR_SECONDARY;
	colors[ImGuiCol_TabUnfocusedActive] = MY_THEME_COLOR_PRIMARY;
	colors[ImGuiCol_Tab] = MY_THEME_COLOR_SECONDARY;
	colors[ImGuiCol_TabHovered] = MY_THEME_COLOR_HOVER;
	colors[ImGuiCol_TabActive] = MY_THEME_COLOR_PRIMARY;
	colors[ImGuiCol_TabUnfocused] = MY_THEME_COLOR_SECONDARY;
	colors[ImGuiCol_TabUnfocusedActive] = MY_THEME_COLOR_PRIMARY;

	style.ChildRounding = 4.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 4.0f;
	style.PopupBorderSize = 1.0f;
	style.WindowBorderSize = 1.0f;
}
