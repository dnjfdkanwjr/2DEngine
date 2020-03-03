#include <iostream>
#include "Window.h"
#include "Timer.h"
#include "Direct3D12Device.h"
auto APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrecInstace, LPWSTR lpCmdLine, int nCmdShow)->int
{

	if (!rp::Window::init(true, hInstance, nCmdShow,
		TEXT("2DEngine Test"),
		TEXT("2DEngine Test"))) {

	}

	MSG msg = {  };

	auto timerInstance = rp::Timer::GetInstance();
	auto device = rp::Direct3D12Device::GetInstance();
	timerInstance->Init();
	device->Init(rp::Window::getHWND(), rp::Window::getWidth(), rp::Window::getHeight());
	while (WM_QUIT != msg.message) {

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {	
			timerInstance->Update();


			std::cout << timerInstance->GetDelta() << std::endl;
		
		}
	}

}


