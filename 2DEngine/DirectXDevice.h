#pragma once
#include <wrl.h>
#include <windows.h>
#include <dxgi1_6.h>
#include <d3d12.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>

#include "Configuration.h"


namespace rp {
    class DirectXDevice
    {
    private:
		
        static Microsoft::WRL::ComPtr<IDXGIFactory4> factory;
		static Microsoft::WRL::ComPtr<IDXGISwapChain3> swapChain;

		static Microsoft::WRL::ComPtr<ID3D12Device> device;
        static Microsoft::WRL::ComPtr<ID3D12Fence>fence;
        static Microsoft::WRL::ComPtr<ID3D12CommandQueue>commandQueue;
        static Microsoft::WRL::ComPtr<ID3D12CommandAllocator>commandAllocator;
        static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
   

        static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap;
        static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap;

		static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeapForDearImGUI;

        static Microsoft::WRL::ComPtr<ID3D12Resource> swapChainBuff[konstant::kBackBufferCount];
        static Microsoft::WRL::ComPtr<ID3D12Resource> dsBuffer;

        static D3D12_VIEWPORT   viewRect;
        static D3D12_RECT       sissorRect;
        static int    currentBackBuffer;
        static UINT64 fenceValue;
        static HANDLE fenceEvent;
		static bool   isTearingSupport;

	private:
		static bool CheckTearingSupport();
    
		static void DearImGuiSetUp(HWND hWnd);
	public:

        static bool Init(HWND hWnd, int width, int height);
        static void PrepareRender(unsigned char r, unsigned char g, unsigned char b);
        static void Render();
        static void Reset(ID3D12PipelineState* pipelineState, bool alloc, bool view = false);
        static void Execute(bool flush);



        static Microsoft::WRL::ComPtr <ID3D12Resource> CreateBuffer(void* initData, UINT64 size, ID3D12Resource** UPBuffer);
        static Microsoft::WRL::ComPtr <ID3D12Device> GetDevice();
        static Microsoft::WRL::ComPtr <ID3D12GraphicsCommandList> GetCommandList();

    };
}

