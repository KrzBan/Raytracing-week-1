#include "Input.h"
#include "Window.h"

#include <array>
#include <vector>

struct InputData {
	std::array<KeyState, GLFW_KEY_LAST> keyStates;
	std::vector<int> releasedKeys;

	double MousePosX = 0.5;
	double MousePosY = 0.5;
	double MouseDeltaX, MouseDeltaY;
	std::shared_ptr<Window> window;
};

static InputData inputData;

void Input::Init(std::shared_ptr<Window> window) {
	glfwSetKeyCallback(window->GetWindow(), Input::InputCallback);
	glfwSetMouseButtonCallback(window->GetWindow(), Input::MouseButtonCallback);
	glfwSetCursorPosCallback(window->GetWindow(), Input::MousePosCallback);

	glfwSetInputMode(window->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(window->GetWindow(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

	inputData.releasedKeys.reserve(GLFW_KEY_LAST + 1);

	inputData.window = window;
}
//TODO: abstract ImGui into itsown layer, add Windows abstraction to handle IsFocused() and similiar things
bool Input::GetKey(int keyCode) { return inputData.keyStates[keyCode].isPressed; }
bool Input::GetKeyDown(int keyCode) { return inputData.keyStates[keyCode].isPressedDown; }
bool Input::GetKeyUp(int keyCode) { return inputData.keyStates[keyCode].released; }

double Input::GetMousePosX() { return inputData.MousePosX; }
double Input::GetMousePosY() { return inputData.MousePosY; }

double Input::GetMouseDeltaX() { return  inputData.MouseDeltaX; }
double Input::GetMouseDeltaY() { return  inputData.MouseDeltaY; }

void Input::InputCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

	switch (action) {
	case GLFW_PRESS:
		inputData.keyStates[key].isPressedDown = true;
		inputData.keyStates[key].isPressed = true;
		inputData.releasedKeys.push_back(key);
		break;
	case GLFW_RELEASE:
		inputData.keyStates[key].isPressedDown = false;;
		inputData.keyStates[key].isPressed = false;
		inputData.keyStates[key].released = true;
		inputData.releasedKeys.push_back(key);
	}
}

void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

	switch (action) {
	case GLFW_PRESS:
		inputData.keyStates[button].isPressed = true;
		inputData.keyStates[button].isPressedDown = true;
		inputData.releasedKeys.push_back(button);

		break;
	case GLFW_RELEASE:
		inputData.keyStates[button].isPressed = false;
		inputData.keyStates[button].isPressedDown = false;
		inputData.keyStates[button].released = true;
		inputData.releasedKeys.push_back(button);
		break;
	}
}

void Input::MousePosCallback(GLFWwindow* window, double xpos, double ypos) {

	inputData.MousePosX = xpos;
	inputData.MousePosY = ypos;
	
}

//TODO: test vector push and clear vs just going through an entire array
void Input::ResetKeyState() {
	inputData.MouseDeltaX = 0;
	inputData.MouseDeltaY = 0;

	for (int i = 0; i <= 7; ++i) {	//Mouse buttons
		if (inputData.keyStates[i].isPressed == true)
			inputData.keyStates[i].isPressedDown = false;
	}

	for (int key : inputData.releasedKeys) {
		inputData.keyStates[key].released = false;
		inputData.keyStates[key].isPressedDown = false;
	}
	inputData.releasedKeys.clear();
}

