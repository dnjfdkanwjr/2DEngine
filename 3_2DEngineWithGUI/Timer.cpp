#include <Windows.h>
#include "Timer.h"

void rp::Timer::Init()
{
	this->curTime = std::chrono::system_clock::now();
	this->prevTime = std::chrono::system_clock::now();
}

void rp::Timer::Update()
{
	this->curTime = std::chrono::system_clock::now();
	this->deltaTime = (this->curTime - this->prevTime);
	this->prevTime = this->curTime;
}

float rp::Timer::GetDelta() const
{
	return this->deltaTime.count();
}
