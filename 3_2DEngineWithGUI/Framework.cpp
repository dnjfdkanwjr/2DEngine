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

	Mesh* mesh = new Mesh();
	Mesh::MakeBoxMesh(1, 1, 1, mesh);
	rendererTest->SetMesh(mesh);
	//rendererTest->SetMesh(Mesh::createGrid(100, 100, 100, 100, 0, 0, 0));
	//rendererTest->SetMesh(Mesh::createGridBox(5,5,5,5,5,5,1,1,1));
}





void rp::RpFramework::Update()
{
	rp::RpTimer::Update();
	//cam->Update();

	PrepareRender();

#ifdef _DEBUG
	cam->GUIRender();
	testObject->GUIRener();
#endif // DEBUG

	rendererTest->Draw(testObject,cam);

	Render();
}

void rp::RpFramework::Exit()
{
	rp::DirectXDevice::WaitForLastFrameGPUSynchronization();
#ifdef _DEBUG
	rp::GraphicUserInterface::Exit();
#endif // DEBUG

}

void rp::RpFramework::PrepareRender()
{
	rp::DirectXDevice::PrepareRender(255, 255, 255,255);
#ifdef _DEBUG
	rp::GraphicUserInterface::PrepareRender();

#endif // DEBUG

}

void rp::RpFramework::Render()
{
	rp::GraphicUserInterface::Render();
	rp::DirectXDevice::Render();
}
