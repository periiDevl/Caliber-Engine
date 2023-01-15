#pragma once
#include<GLFW/glfw3.h>

class Setup
{
public:

    bool IsError;
    // Default constructor for the Component class
    Setup()
    {
        // Initialize the object counter to 0
        //objects = 0;

        // Initialize all elements in the OSWsave array to default values

    }

   

    void SETUP_GLFW()
    {
        glfwInit();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    }


    

    void SETUP_IMGUI(GLFWwindow* window)
    {
        // Initialize ImGUI
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");
    }

};
