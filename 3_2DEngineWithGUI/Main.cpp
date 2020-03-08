#include <iostream>
#include "Window.h"
#include "Timer.h"
#include "DirectXDevice.h"

auto APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrecInstace, LPWSTR lpCmdLine, int nCmdShow)->int
{

	if (!rp::Window::init(true, hInstance, nCmdShow,
		TEXT("2DEngine Test"),
		TEXT("2DEngine Test"))) {
	}

	if (rp::DirectXDevice::Init(rp::Window::getHWND(), rp::Window::getWidth(), rp::Window::getHeight())) {
		std::cout << "Direct X 12 Init Success" << std::endl;
	}

	MSG msg = {  };

	auto timerInstance = rp::Timer::GetInstance();
	timerInstance->Init();

	while (WM_QUIT != msg.message) {

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			continue;
		}
		rp::DirectXDevice::PrepareRender(25, 25, 25);

		rp::DirectXDevice::Render();

	}

}


