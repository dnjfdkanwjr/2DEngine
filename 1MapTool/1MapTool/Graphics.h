#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <d3d11.h>
#include <string_view>

#include "../../0Library/imgui/imgui.h"
#include "../../0Library/imgui/imgui_impl_win32.h"
#include "../../0Library/imgui/imgui_impl_dx11.h"

static class Graphics
{
public:
	static bool initialize();
	static void finalize();
	static void render();
	static void present();

	static bool createDeviceD(HWND hWnd);
	static void cleanupDeviceD();
	static void createRenderTarget();
	static void cleanupRenderTarget();
	static void loadFont(const std::string_view& font_path);

	static ID3D11Device*			getDevice();
	static ID3D11DeviceContext*		getDeviceContext();
	static IDXGISwapChain*			getSwapChain();
	static ID3D11RenderTargetView*	getRenderTargetView();

	static ImGuiIO&	getIO();

private:
	static ID3D11Device*			device;
	static ID3D11DeviceContext*		device_context;
	static IDXGISwapChain*			swap_chain;
	static ID3D11RenderTargetView*	render_target_view;
	static ImFont*					font;
	static ImVec4					bg_color;
	static ImGuiIO					imgui_io;
};