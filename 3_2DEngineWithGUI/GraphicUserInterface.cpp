#include <iostream>
#include <string>
#include "Timer.h"
#include "GraphicUserInterface.h"

#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_win32.h"
#include "IMGUI/imgui_impl_dx12.h"

Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rp::GraphicUserInterface::srvDescHeap{};
Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> rp::GraphicUserInterface::cachedCommandList{};

void rp::GraphicUserInterface::Init(HWND hwnd)
{

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	desc.NumDescriptors = 1;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	if (rp::DirectXDevice::GetDevice()->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&srvDescHeap)) != S_OK) {
		std::cout << "Fail Creattion Descritor Heap In IMGUI" << std::endl;
		return;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX12_Init(
		rp::DirectXDevice::GetDevice(),
		konstant::kNumberOfAllocators,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		srvDescHeap.Get(),
		srvDescHeap.Get()->GetCPUDescriptorHandleForHeapStart(),
		srvDescHeap.Get()->GetGPUDescriptorHandleForHeapStart()
	);

	rp::GraphicUserInterface::cachedCommandList = rp::DirectXDevice::GetCommandList();

}


void rp::GraphicUserInterface::PrepareRender()
{
	//static bool show_demo_window = true;
	//static bool show_another_window = true;

	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//{
	//	ImGui::Begin("Hello, world!");
	//	const std::string baseString{ "currentFrame  " };
	//	auto tempString = baseString + std::to_string(1.f / rp::RpTimer::GetDelta());
	//	ImGui::Text(tempString.c_str());
	//	ImGui::End();
	//}


	//if (show_another_window)
	//{
	//	ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
	//	ImGui::Text("Hello from another window!");
	//	if (ImGui::Button("Close Me"))
	//		show_another_window = false;
	//	ImGui::End();
	//}
}


void rp::GraphicUserInterface::Render()
{
	rp::GraphicUserInterface::cachedCommandList->SetDescriptorHeaps(1, srvDescHeap.GetAddressOf());
	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), rp::GraphicUserInterface::cachedCommandList.Get());
}

void rp::GraphicUserInterface::Exit()
{

	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

}
