#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

#include "d3dx12.h"
#include "Configuration.h"
#include "Singleton.h"


namespace rp {

	struct CommandList {
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator>  commandAllocator;
	};

	class Direct3D12Device : public Singleton<Direct3D12Device>
	{
	private:
		Microsoft::WRL::ComPtr<ID3D12Fence> fence;

		Microsoft::WRL::ComPtr<IDXGISwapChain3> swapChain;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> renderTargetViewHeap;

		Microsoft::WRL::ComPtr<ID3D12Device> device;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;


		std::shared_ptr<CommandList> commandLists[constant::maxFrameCount];

		Microsoft::WRL::ComPtr<ID3D12Resource> renderTargets[constant::maxFrameCount];

		uint currentFrameIndex;
		HANDLE fenceEvent;
		UINT64 m_fenceValues[constant::maxFrameCount];

		CD3DX12_VIEWPORT viewPort;
		CD3DX12_RECT scissorRect;

	public:
		
		void Init(HWND handleWindow,int width, int height);
	};
}

