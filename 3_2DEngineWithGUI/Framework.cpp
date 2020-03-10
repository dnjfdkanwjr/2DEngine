#include <iostream>

#include "Framework.h"
#include "Window.h"
#include "Timer.h"



void rp::RpFramework::Init()
{

	if (rp::DirectXDevice::Init(rp::Window::getHWND(), rp::Window::getWidth(), rp::Window::getHeight())) {
		std::cout << "Direct X 12 Init Success" << std::endl;
	}

	rp::GraphicUserInterface::Init(rp::Window::getHWND());
	rp::RpTimer::Init();
}




void rp::RpFramework::Update()
{
	rp::RpTimer::Update();

	PrepareRender();
	Render();
}

void rp::RpFramework::PrepareRender()
{
	rp::DirectXDevice::PrepareRender(25, 25, 25,255);
	rp::GraphicUserInterface::PrepareRender();
}

void rp::RpFramework::Render()
{
	rp::GraphicUserInterface::Render();
	rp::DirectXDevice::Render();
}
