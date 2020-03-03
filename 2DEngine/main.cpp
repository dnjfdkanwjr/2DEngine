#include <iostream>
#include "Window.h"
#include "Timer.h"

auto APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrecInstace, LPWSTR lpCmdLine, int nCmdShow)->int
{

	if (!rp::Window::init(true, hInstance, nCmdShow,
		TEXT("2DEngine Test"),
		TEXT("2DEngine Test"))) {

	}

	MSG msg = {  };

	auto timerInstance = rp::Timer::GetInstance();

	timerInstance->Init();
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


