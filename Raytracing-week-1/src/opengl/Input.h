#pragma once

#include <iostream>

#include "KeyCodes.h"
#include "Utility.h"

struct KeyState {
	bool isPressedDown; //one frame down
	bool isPressed;	//just down
	bool released;	//released
};

class Window;

class Input {
public:
	static void Init(std::shared_ptr<Window> window);

	static bool GetKey(int keyCode);
	static bool GetKeyDown(int keyCode);
	static bool GetKeyUp(int keyCode);	//Repeat cleans Down state, Release cleans Repeat state, nothing resets Release state, TODO: add clean-up method

	static double GetMousePosX();
	static double GetMousePosY();

	static double GetMouseDeltaX();
	static double GetMouseDeltaY();

	static void InputCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MousePosCallback(GLFWwindow* window, double xpos, double ypos);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void ResetKeyState();	//put BEFORE glfwPollEvents, or nothing happens
};


