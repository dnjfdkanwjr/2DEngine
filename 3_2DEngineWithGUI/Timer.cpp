#include <Windows.h>
#include "Timer.h"

std::chrono::time_point<std::chrono::high_resolution_clock>	 rp::RpTimer::curTime{};
std::chrono::time_point<std::chrono::high_resolution_clock>	 rp::RpTimer::prevTime{};
std::chrono::duration<float>						 rp::RpTimer::deltaTime{};


void rp::RpTimer::Init()
{
	curTime = std::chrono::high_resolution_clock::now();
	prevTime = std::chrono::high_resolution_clock::now();
}

void rp::RpTimer::Update()
{
	curTime = std::chrono::high_resolution_clock::now();
	deltaTime = (curTime - prevTime);
	prevTime = curTime;
}


float rp::RpTimer::GetDelta()
{
	return deltaTime.count();
}
