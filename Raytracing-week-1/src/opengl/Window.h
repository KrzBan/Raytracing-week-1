#pragma once

#include <iostream>

#include "Utility.h"

static GLFWwindow* OpenGLInit(int width, int height, std::string name);

class Window
{
public:
	Window(uint32_t scrWidth, uint32_t scrHeight, const char* windowName)
		: m_Width(scrWidth), m_Height(scrHeight), m_WindowName(windowName), m_IsVSyncOn(false), m_IsFocused(true) {
		m_Window = OpenGLInit(m_Width, m_Height, m_WindowName);

		/*
		//Setup ImGui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		// Setup Platform/Renderer bindings, BEFORE Input::Init, for now
		ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		*/
		
	}

	~Window() {
		/*
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		*/
		
		glfwTerminate();
	}

	void OnUpdate() {
		/*
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		*/
		

		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}
	/*
	void ImGuiNewFrame() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}
	*/
	
	const uint32_t GetWidth() const { return m_Width; }
	const uint32_t GetHeight() const { return m_Height; }

	const bool IsFocused() const { return m_IsFocused; }

	GLFWwindow* GetWindow() const { return m_Window; }

	void SetVSync(bool set) const {
		glfwSwapInterval(set);
		m_IsVSyncOn = set;
	}
	const bool GetVSync() const { return m_IsVSyncOn; }

	void Clear() const {
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void SetTitle(std::string title) {
		glfwSetWindowTitle(m_Window, title.c_str());
	}

private:
	uint32_t m_Width;
	uint32_t m_Height;
	std::string m_WindowName;

	mutable bool m_IsVSyncOn;

	bool m_IsFocused;

	GLFWwindow* m_Window;
};

GLFWwindow* OpenGLInit(int width, int height, std::string name) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
	if (window == nullptr) {
		std::cerr << "Failed to create GLFW window\n";
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD\n";
		glfwTerminate();
		return nullptr;
	}

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, width, height);

	return window;
}