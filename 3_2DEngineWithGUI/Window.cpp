#include <string>
#include <iostream>
#include "Window.h"
#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_dx12.h"
#include "DirectXDevice.h"
bool						rp::Window::debug_console;
HINSTANCE					rp::Window::handle_instance;
WNDCLASSEX					rp::Window::window_class_ex;
HWND						rp::Window::handle_window;
int							rp::Window::width = 0;
int							rp::Window::height = 0;
int							rp::Window::max_threads = 0;
std::wstring				rp::Window::class_name;
std::wstring				rp::Window::application_name;
float						rp::Window::frame_set_timer = 3.0f;
int							rp::Window::drawed_frame = 0;

int							rp::Window::fps{};

float							rp::Window::cam_x = 30;
float							rp::Window::cam_z = 45;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK rp::windowProcedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_SIZE:
		if (!rp::DirectXDevice::GetDevice()) {
			return 0;
		}

		if (wParam != SIZE_MINIMIZED) {
			ImGui_ImplDX12_InvalidateDeviceObjects();
			//CleanupRenderTarget();
			//rp::DirectXDevice::ResizeSwapChain(hWnd, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
		
			ImGui_ImplDX12_CreateDeviceObjects();
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_MOUSEWHEEL:

			break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

bool rp::Window::init(bool Debug, HINSTANCE hInstance, int nCmdShow, LPCWSTR CLSName, LPCWSTR APPName,UINT max_threads)
{

	RECT actualDesktop;
	GetWindowRect(GetDesktopWindow(), &actualDesktop);

	rp::Window::handle_instance = hInstance;

	rp::Window::setWidth(1400);
	rp::Window::setHeight(800);

	rp::Window::class_name = (CLSName);
	rp::Window::application_name = (APPName);
	rp::Window::debug_console = (Debug);
	rp::Window::max_threads = max_threads;
	rp::Window::window_class_ex.cbSize = sizeof(WNDCLASSEX);
	rp::Window::window_class_ex.style = CS_HREDRAW | CS_VREDRAW;
	rp::Window::window_class_ex.lpfnWndProc = windowProcedure;
	rp::Window::window_class_ex.cbClsExtra = 0;
	rp::Window::window_class_ex.cbWndExtra = 0;
	rp::Window::window_class_ex.hInstance = handle_instance;
	rp::Window::window_class_ex.hIcon = NULL;
	rp::Window::window_class_ex.hCursor = LoadCursor(NULL, IDC_ARROW);
	rp::Window::window_class_ex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	rp::Window::window_class_ex.lpszMenuName = NULL;
	rp::Window::window_class_ex.lpszClassName = rp::Window::class_name.c_str();
	rp::Window::window_class_ex.hIconSm = NULL;

	


	if (!RegisterClassEx(&rp::Window::window_class_ex)) return false;

	rp::Window::handle_window = CreateWindowEx(0
		,rp::Window::class_name.c_str(),
		rp::Window::application_name.c_str(),
		WS_OVERLAPPEDWINDOW, //전체모드시 WS_POPUP	this is popup
		CW_USEDEFAULT,       //전체모드시 0	this is popup
		CW_USEDEFAULT,       //전체모드시 0 this is popup
		rp::Window::width,
		rp::Window::height,
		nullptr,
		nullptr,
		rp::Window::handle_instance,
		nullptr);

	ShowWindow(rp::Window::handle_window, nCmdShow);

	if (rp::Window::debug_console) START_CONSOLE();


	ShowCursor(true);

	return true;
}



bool rp::Window::exit()
{
	if (rp::Window::debug_console) STOP_CONSOLE();

	return true;
}

void rp::Window::update(float& updateTimer)
{	


}



