#include <iostream>

#include "Framework.h"
#include "Window.h"
#include "Timer.h"
#include "GraphicResourceMananger.h"
#include "RpGameObject.h"

#include "Mesh.h"

rp::QuadRenderer* rp::RpFramework::rendererTest{};
rp::RpGameObject* rp::RpFramework::testObject{};

rp::Camera* rp::RpFramework::cam{};
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
	cam = new rp::Camera();

	rendererTest->SetMesh(Mesh::createGridBox(5, 5, 5, 5, 5, 5, 1, 1, 1));
	//rendererTest->SetMesh(Mesh::createGrid(100, 100, 2, 5, 5, 5, 1));
}





void rp::RpFramework::Update()
{
	rp::RpTimer::Update();

	cam->Update();

	PrepareRender();

	rp::DirectXDevice::GetCommandList()->SetGraphicsRootConstantBufferView(1, cam->GetConstBuffer().GetUploadBuffer()->GetGPUVirtualAddress());
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
	rp::DirectXDevice::PrepareRender(0, 0, 0,255);
	rp::GraphicUserInterface::PrepareRender();
}

void rp::RpFramework::Render()
{
	rp::GraphicUserInterface::Render();
	rp::DirectXDevice::Render();
}
