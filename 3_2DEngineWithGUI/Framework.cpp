#include <iostream>

#include "Framework.h"
#include "Window.h"
#include "Timer.h"
#include "GraphicResourceMananger.h"
#include "RpGameObject.h"

#include "Mesh.h"

rp::QuadRenderer* rp::RpFramework::rendererTest{};
rp::RpGameObject* rp::RpFramework::testObject{};
void rp::RpFramework::Init()
{
	if (rp::DirectXDevice::Init(rp::Window::getHWND(), rp::Window::getWidth(), rp::Window::getHeight())) {
		std::cout << "Direct X 12 Init Success" << std::endl;
	}

	rp::GraphicResourceMananger::Init();

	rp::GraphicUserInterface::Init(rp::Window::getHWND());
	rp::RpTimer::Init();

	rendererTest = new rp::QuadRenderer();
	testObject = new rp::RpGameObject();
	rendererTest->SetMesh(Mesh::createGridBox(1, 1, 1, 5, 5, 5, 1, 1, 1));
}





void rp::RpFramework::Update()
{

	rp::RpTimer::Update();
	PrepareRender();
	rendererTest->Draw(testObject);
	Render();
}

void rp::RpFramework::Exit()
{
	rp::DirectXDevice::WaitForLastFrameGPUSynchronization();
	rp::GraphicUserInterface::Exit();
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
