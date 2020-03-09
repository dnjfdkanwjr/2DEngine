#pragma once
#include <wrl.h>
#include <windows.h>
#include <dxgi1_6.h>
#include <d3d12.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <vector>
#include "Configuration.h"


namespace rp {


	class AllocatorWithFence {
	private:

		Microsoft::WRL::ComPtr<ID3D12CommandAllocator>commandAllocator{};
		UINT64 lastFenceValue{};

	public:
		AllocatorWithFence(const D3D12_COMMAND_QUEUE_DESC& queueDesc, ID3D12Device* device) {
			if (device->CreateCommandAllocator(queueDesc.Type, __uuidof(ID3D12CommandAllocator), (void**)&commandAllocator) != S_OK)
			{
			
			}

		}

		inline Microsoft::WRL::ComPtr<ID3D12CommandAllocator> GetAllocator() const {
			return commandAllocator;
		};

		inline UINT64 GetFenceValue() const {
			return lastFenceValue;
		}

		inline void SetFenceValue(UINT64 fenceValue) {
			lastFenceValue = fenceValue;
		}
	};

    class DirectXDevice
    {
    private:
		
        static Microsoft::WRL::ComPtr<IDXGIFactory4> factory;
		static Microsoft::WRL::ComPtr<IDXGISwapChain3> swapChain;

		static Microsoft::WRL::ComPtr<ID3D12Device> device;
        static Microsoft::WRL::ComPtr<ID3D12Fence>fence;
        static Microsoft::WRL::ComPtr<ID3D12CommandQueue>commandQueue;

        //static Microsoft::WRL::ComPtr<ID3D12CommandAllocator>commandAllocator;

		static std::vector<AllocatorWithFence> commandAllocators;

        static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
   

        static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap;
        static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap;

		static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvHeapForDearImGUI;

        static Microsoft::WRL::ComPtr<ID3D12Resource> swapChainBuff[konstant::kBackBufferCount];
        static Microsoft::WRL::ComPtr<ID3D12Resource> dsBuffer;

        static D3D12_VIEWPORT   viewRect;
        static D3D12_RECT       sissorRect;
        static int    currentBackBuffer;
		static uint currentAllocatorIndex;
		static UINT64 fenceValue;
		static bool   isTearingSupport;

	private:
		static bool CheckTearingSupport();
    
		static void DearImGuiSetUp(HWND hWnd);

		static void FlushCommands();
	public:

        static bool Init(HWND hWnd, int width, int height);

		static void WaitForLastFrameGPUSynchronization();
		static void WaitForNextFrameGPUSynchronization();

        static void PrepareRender(unsigned char r, unsigned char g, unsigned char b);
		static void Render();


        static void Execute();



        static Microsoft::WRL::ComPtr <ID3D12Resource> CreateBuffer(void* initData, UINT64 size, ID3D12Resource** UPBuffer);
        static Microsoft::WRL::ComPtr <ID3D12Device> GetDevice();
        static Microsoft::WRL::ComPtr <ID3D12GraphicsCommandList> GetCommandList();

		static void ResizeSwapChain(HWND hWnd, int width, int height);

    };
}

