#pragma once

#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
void SETUI(bool& no_resize, bool& no_move, bool& run, unsigned int& postProcessingTexture, unsigned int& shadowMap, GLuint& depthTexture,
	static int& colorChoice, int& msaaSamples, float& FXAA_SPAN_MAX, float& FXAA_REDUCE_MIN, float& FXAA_REDUCE_MUL,
	Shader& framebufferProgram, float& gamma, float& exposure, int& bloom, float& BloomSpreadBlur, Shader& blurProgram, 
	int& shadowMapWidth, int& shadowMapHeight, bool& bakeShadows, bool& renderShadows, int&	shadowSampleRadius,
	float& avgShadow, float& DepthBias1, float& DepthBias2, Shader& shaderProgram, float& fogNear, float& viewFarPlane,
	bool& enableskybox, bool& vsync, float& highCameraSpeed, float& cameraNormalSpeed, bool& wireframe, bool& BPL_LIGHTING, int maxmsaa) {





	ImGui::Begin("Preferences", 0, (no_resize ? ImGuiWindowFlags_NoResize : 0) | (no_move ? ImGuiWindowFlags_NoMove : 0));
	{

		ImGui::BeginTabBar("Preferences Bar");
		if (ImGui::BeginTabItem("Styles")) {


			if (ImGui::RadioButton("Default", &colorChoice, 0) || colorChoice == 0)
			{
				DefaultTheme();
			}
			if (ImGui::RadioButton("Light", &colorChoice, 1) || colorChoice == 1)
			{
				LightTheme();
			}
			if (ImGui::RadioButton("Future", &colorChoice, 2) || colorChoice == 2)
			{
				FutureTheme();
			}
			if (ImGui::RadioButton("Hacker", &colorChoice, 3) || colorChoice == 3)
			{
				HackerTheme();
			}
			if (ImGui::RadioButton("Cyan_Blue", &colorChoice, 4) || colorChoice == 4)
			{
				Cyan_Blue_Theme();
			}
			if (ImGui::RadioButton("Jonayes Theme", &colorChoice, 5) || colorChoice == 5)
			{
				JonayesTheme();
			}
			if (ImGui::RadioButton("EyeSore (For all of you weirdoz)", &colorChoice, 6) || colorChoice == 6)
			{
				EyESoRETheme();
			}
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Layout")) {
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
			ImGui::Text("Values here will not be saved.");
			ImGui::PopStyleColor();
			ImGui::Checkbox("No Window Moving", &no_move);
			ImGui::Checkbox("No Window Resize", &no_resize);
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();

	}
	ImGui::End();

	//ImGui::Begin("background", 0, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);






	if (ImGui::Begin("project settings", 0, (no_resize ? ImGuiWindowFlags_NoResize : 0) | (no_move ? ImGuiWindowFlags_NoMove : 0))) {
		if (ImGui::BeginTabBar("project tabs"))
		{

			if (ImGui::BeginTabItem("Graphics"))
			{
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
				ImGui::Text("This will change how your game looks, make sure you have the proper graphics card.");
				ImGui::PopStyleColor();

				if (ImGui::CollapsingHeader("Anti-Aliasing", ImGuiTreeNodeFlags_DefaultOpen)) {
					ImGui::Columns(2, nullptr, true);

					ImGui::Text("Multisample Anti-Aliasing (Needs restart to change)");
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
					ImGui::Text("MSAA Needs restart to apply it.");
					ImGui::Text(("MAX : " + std::to_string(maxmsaa)).c_str());

					ImGui::PopStyleColor();
					ImGui::InputInt("MSSA Samples", &msaaSamples);

					ImGui::NextColumn();
					ImGui::Text("Fast Approximate Anti-Aliasing");
					ImGui::InputFloat("FXAA_SPAN_MAX", &FXAA_SPAN_MAX);
					ImGui::InputFloat("FXAA_REDUCE_MIN", &FXAA_REDUCE_MIN);
					ImGui::InputFloat("FXAA_REDUCE_MUL", &FXAA_REDUCE_MUL);

					ImGui::Columns(1, nullptr, false);
				}

				ImGui::Separator();

				if (ImGui::CollapsingHeader("Post-Processing")) {
					ImGui::InputFloat("Gamma correction value", &gamma, 0.3f, 1, "%.3f", 0);
					ImGui::InputFloat("Exposure value", &exposure, 0.3f, 1, "%.3f", 0);
					if (ImGui::CollapsingHeader("Bloom settings")) {
						ImGui::InputInt("Bloom Amount", &bloom, 1, 100);
						ImGui::InputFloat("Bloom Spread", &BloomSpreadBlur);
					}


				}
				ImGui::Separator();


				if (ImGui::CollapsingHeader("Lighting", ImGuiTreeNodeFlags_DefaultOpen)) {
					ImGui::Checkbox("BPL_LIGHTING", &BPL_LIGHTING);
					ImGui::InputInt("Shadow Map Width", &shadowMapWidth, 1, 100);
					ImGui::InputInt("Shadow Map Height", &shadowMapHeight, 1, 100);
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
					ImGui::Text("Baking the shadow's will not improve the quality it will only freeze the texture");
					ImGui::PopStyleColor();
					ImGui::Checkbox("Bake/Static Shadows", &bakeShadows);
					if (!bakeShadows) {
						ImGui::Checkbox("Enable shadows", &renderShadows);
					}
					ImGui::InputInt("Sample Radius", &shadowSampleRadius, 0, 100);

					ImGui::InputFloat("Average Shadow (Transparency)", &avgShadow);
					ImGui::InputFloat("Depth Bias 1", &DepthBias1);
					ImGui::InputFloat("Depth Bias 2", &DepthBias2);
					shaderProgram.Activate();
					glUniform1f(glGetUniformLocation(shaderProgram.ID, "avgShadow"), avgShadow);
					glUniform1f(glGetUniformLocation(shaderProgram.ID, "bias1"), DepthBias1);
					glUniform1f(glGetUniformLocation(shaderProgram.ID, "bias2"), DepthBias2);
				}
				ImGui::Separator();

				if (ImGui::CollapsingHeader("Atmospheric")) {




					ImGui::SliderFloat("Fog Near Value", &fogNear, 0, 3, "%.3f", 0);
					ImGui::InputFloat("Far Plane View distance", &viewFarPlane, 1, 30);
					shaderProgram.Activate();
					glUniform1f(glGetUniformLocation(shaderProgram.ID, "near"), fogNear);
					glUniform1f(glGetUniformLocation(shaderProgram.ID, "far"), viewFarPlane);


					ImGui::Checkbox("Enable Skybox", &enableskybox);
				}
				ImGui::Separator();

				if (ImGui::CollapsingHeader("Screen", ImGuiTreeNodeFlags_DefaultOpen)) {
					ImGui::Checkbox("Enable Vsync", &vsync);
				}










				ImGui::EndTabItem();

			}

			if (ImGui::BeginTabItem("Viewport settings"))
			{
				ImGui::InputFloat("Shift Speed", &highCameraSpeed, 0.3f, 1, "%.3f", 0);
				ImGui::InputFloat("Normal Speed", &cameraNormalSpeed, 0.3f, 1, "%.3f", 0);

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Debug"))
			{
				ImGui::Checkbox("Enable Wireframe", &wireframe);
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();

		}

		ImGui::End();

	}
}