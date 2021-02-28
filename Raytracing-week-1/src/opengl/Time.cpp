#include "Time.h"

#include "Utility.h"

struct TimeData {
	double lastTime = 0;
	double currentTime = 0;
	double deltaTime = 0;
};

static TimeData timeData;

double Time::DeltaTime() {
	return timeData.deltaTime;
}

double Time::RealTime() {
	return timeData.currentTime;
}

float Time::DeltaTimeF() {
	return static_cast<float>(timeData.deltaTime);
}

float Time::RealTimeF() {
	return static_cast<float>(timeData.currentTime);
}

void Time::UpdateTime() {
	timeData.currentTime = glfwGetTime();

	timeData.deltaTime = timeData.currentTime - timeData.lastTime;

	timeData.lastTime = timeData.currentTime;
}

int Time::FPS() {
	return static_cast<int>(1.0 / timeData.deltaTime);
}
