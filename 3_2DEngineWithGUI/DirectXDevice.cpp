#include <iostream>
#include <thread>
#include "DirectXDevice.h"
#include "DirectXHelper.h"
#include "d3dx12.h"




#pragma comment ( lib, "d3d12.lib")
#pragma comment ( lib, "D3DCompiler.lib")
#pragma comment ( lib, "dxgi.lib")

using namespace rp;

Microsoft::WRL::ComPtr<IDXGIFactory4> rp::DirectXDevice::factory{};
Microsoft::WRL::ComPtr<IDXGISwapChain3> rp::DirectXDevice::swapChain{};

Microsoft::WRL::ComPtr<ID3D12Device> rp::DirectXDevice::device{};
Microsoft::WRL::ComPtr<ID3D12Fence> rp::DirectXDevice::fence{};
Microsoft::WRL::ComPtr<ID3D12CommandQueue>rp::DirectXDevice::commandQueue{};

std::vector<AllocatorWithFence> rp::DirectXDevice::commandAllocators{};

Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> rp::DirectXDevice::commandList{};


Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rp::DirectXDevice::rtvHeap{};
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rp::DirectXDevice::dsvHeap{};


Microsoft::WRL::ComPtr<ID3D12Resource> rp::DirectXDevice::swapChainBuff[konstant::kBackBufferCount]{};
Microsoft::WRL::ComPtr<ID3D12Resource> rp::DirectXDevice::dsBuffer{};

D3D12_VIEWPORT   rp::DirectXDevice::viewRect{};
D3D12_RECT       rp::DirectXDevice::sissorRect{};
uint    rp::DirectXDevice::currentBackBuffer{ 0 };
uint	rp::DirectXDevice::currentAllocatorIndex{};
UINT64 rp::DirectXDevice::fenceValue{0};
bool   rp::DirectXDevice::isTearingSupport{false};

bool rp::DirectXDevice::CheckTearingSupport()
{

#ifndef PIXSUPPORT
		ComPtr<IDXGIFactory6> factory;
		HRESULT hr = CreateDXGIFactory1(IID_PPV_ARGS(&factory));
		BOOL allowTearing = FALSE;
		if (SUCCEEDED(hr))
		{
			hr = factory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing));
		}

		return SUCCEEDED(hr) && allowTearing;
#else
	return true;
#endif
	
}



void rp::DirectXDevice::FlushCommands()
{
}

bool DirectXDevice::Init(HWND hWnd, int width, int height)
{
	UINT dxgiFactoryFlags = 0;
	using Microsoft::WRL::ComPtr;

#if defined(_DEBUG)
	// Enable the debug layer (requires the Graphics Tools "optional feature").
	// NOTE: Enabling the debug layer after device creation will invalidate the active device.
	{
		ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();

			// Enable additional debug layers.
			dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		}
	}
#endif

	ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)));

    if (D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), (void**)&device) != S_OK)
    {
        return false;
    }


    if (device->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)&fence) != S_OK)
    {
        return false;
    }


    D3D12_COMMAND_QUEUE_DESC queueDesc;
    ZeroMemory(&queueDesc, sizeof(D3D12_COMMAND_QUEUE_DESC));

    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;

    if (device->CreateCommandQueue(&queueDesc, __uuidof(ID3D12CommandQueue), (void**)&commandQueue) != S_OK)
    {
        return false;
    }


	for (int i = 0; i < konstant::kNumberOfAllocators; ++i) {
		commandAllocators.emplace_back(queueDesc, device.Get());
	}


    if (device->CreateCommandList(0, queueDesc.Type, commandAllocators[currentAllocatorIndex].GetAllocator().Get(), NULL, __uuidof(ID3D12GraphicsCommandList), (void**)&commandList) != S_OK)
    {
        return false;
    }


	isTearingSupport = CheckTearingSupport();

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = konstant::kBackBufferCount;
	swapChainDesc.Width = width;
	swapChainDesc.Height = height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;

	// It is recommended to always use the tearing flag when it is available.
	swapChainDesc.Flags = isTearingSupport ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

	ComPtr<IDXGISwapChain1> swapChainLocal;

	ThrowIfFailed(factory->CreateSwapChainForHwnd(
		commandQueue.Get(),        // Swap chain needs the queue so that it can force a flush on it.
		hWnd,
		&swapChainDesc,
		nullptr,
		nullptr,
		&swapChainLocal
	));

	if (isTearingSupport)
	{
		// When tearing support is enabled we will handle ALT+Enter key presses in the
		// window message loop rather than let DXGI handle it by calling SetFullscreenState.
		factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);
	}

	ThrowIfFailed(swapChainLocal.As(&swapChain));


    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
    ZeroMemory(&rtvHeapDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));
    rtvHeapDesc.NumDescriptors = konstant::kBackBufferCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;


    if (device->CreateDescriptorHeap(&rtvHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&rtvHeap) != S_OK)
    {

        return false;
    }



    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
    ZeroMemory(&dsvHeapDesc, sizeof(D3D12_DESCRIPTOR_HEAP_DESC));

    dsvHeapDesc.NumDescriptors = 1;
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    if (device->CreateDescriptorHeap(&dsvHeapDesc, __uuidof(ID3D12DescriptorHeap), (void**)&dsvHeap) != S_OK)
    {
        return false;
    }



    //desc heap
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();

    global::gRtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	global::gCbvSrvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    for (int i = 0; i < konstant::kBackBufferCount; i++)
    {
        swapChain->GetBuffer(i, __uuidof(ID3D12Resource), (void**)&swapChainBuff[i]);
        device->CreateRenderTargetView(swapChainBuff[i].Get(), NULL, rtvHandle);

        rtvHandle.ptr += global::gRtvDescriptorSize;
    }


    D3D12_RESOURCE_DESC dsDesc;
    ZeroMemory(&dsDesc, sizeof(D3D12_RESOURCE_DESC));

    dsDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    dsDesc.Width = width;
    dsDesc.Height = height;
    dsDesc.DepthOrArraySize = 1;
    dsDesc.MipLevels = 1;
    dsDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsDesc.SampleDesc.Count = 1;
    dsDesc.SampleDesc.Quality = 0;
    dsDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    dsDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;


    D3D12_CLEAR_VALUE clearValue;
    ZeroMemory(&clearValue, sizeof(D3D12_CLEAR_VALUE));

    clearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    clearValue.DepthStencil.Depth = 1.0f;
    clearValue.DepthStencil.Stencil = 0;

    D3D12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

    if (device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &dsDesc, D3D12_RESOURCE_STATE_COMMON, &clearValue, __uuidof(ID3D12Resource), (void**)&dsBuffer) != S_OK)
    {

        return false;
    }


	viewRect = { 0.0f, 0.0f, (float)width, float(height), 0.0f, 1.0f };
	sissorRect = { 0, 0,width, height };



    D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
    device->CreateDepthStencilView(dsBuffer.Get(), NULL, dsvHandle); 

    D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(dsBuffer.Get(),
        D3D12_RESOURCE_STATE_COMMON,
        D3D12_RESOURCE_STATE_DEPTH_WRITE);


	//Temporary Codes For Making Resource
	auto alloc = commandAllocators[currentAllocatorIndex].GetAllocator().Get();
	alloc->Reset();

	commandList->Reset(alloc, nullptr);
    commandList->ResourceBarrier(1, &barrier);

	Execute();
	WaitForLastFrameGPUSynchronization();

    return true;
}



void rp::DirectXDevice::WaitForLastFrameGPUSynchronization()
{
	while (fence.Get()->GetCompletedValue() < commandAllocators[currentAllocatorIndex].GetFenceValue())
	{
		std::this_thread::yield();
	}
}

void rp::DirectXDevice::WaitForNextFrameGPUSynchronization()
{
	currentAllocatorIndex = ( currentAllocatorIndex + 1 ) % konstant::kNumberOfAllocators;
	WaitForLastFrameGPUSynchronization();
}

void DirectXDevice::PrepareRender(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	WaitForNextFrameGPUSynchronization();
	auto alloc = commandAllocators[currentAllocatorIndex].GetAllocator().Get();
	alloc->Reset();

	commandList->Reset(alloc,  nullptr);	

	currentBackBuffer = swapChain->GetCurrentBackBufferIndex();

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
	rtvHandle.ptr += (unsigned long long)global::gRtvDescriptorSize * currentBackBuffer;

	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = swapChainBuff[currentBackBuffer].Get();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	static float clearColor[4] = { (float)r/ 255.f,(float)g / 255.f,(float)b / 255.f, (float)a / 255.f, };

	commandList->ResourceBarrier(1, &barrier);

	commandList->ClearRenderTargetView(rtvHandle, (float*)&clearColor, 0, NULL);
	commandList->OMSetRenderTargets(1, &rtvHandle, true, &dsvHandle);
	commandList->RSSetViewports(1, &viewRect);
	commandList->RSSetScissorRects(1, &sissorRect);
}


void DirectXDevice::Render()
{
	D3D12_RESOURCE_BARRIER barrier{ CD3DX12_RESOURCE_BARRIER::Transition(swapChainBuff[currentBackBuffer].Get(),D3D12_RESOURCE_STATE_RENDER_TARGET,D3D12_RESOURCE_STATE_PRESENT) };
    commandList->ResourceBarrier(1, &barrier);
	Execute();
	swapChain->Present(0, 0);

}


void DirectXDevice::Execute()
{
    commandList->Close();
    ID3D12CommandList* cmdLists[1] = { commandList.Get()};	
    commandQueue->ExecuteCommandLists(1, cmdLists);
	ThrowIfFailed(commandQueue->Signal(fence.Get(), ++fenceValue));
	commandAllocators[currentAllocatorIndex].SetFenceValue(fenceValue);
}


Microsoft::WRL::ComPtr<ID3D12Resource> DirectXDevice::CreateBuffer(void* initData, UINT64 size, ID3D12Resource** UPBuffer)
{

    //Reset(NULL, true);

    ID3D12Resource* defaultBuffer = NULL;
    D3D12_RESOURCE_DESC   buffer = CD3DX12_RESOURCE_DESC::Buffer(size);

    D3D12_HEAP_PROPERTIES defaultHeap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);


    if (device->CreateCommittedResource(&defaultHeap,
        D3D12_HEAP_FLAG_NONE,
        &buffer,
        D3D12_RESOURCE_STATE_COMMON,
        nullptr,
        __uuidof(ID3D12Resource), (void**)&defaultBuffer) != S_OK)
    { 
        return NULL;
    }



    D3D12_HEAP_PROPERTIES uploadHeap = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

    if (device->CreateCommittedResource(&uploadHeap,
        D3D12_HEAP_FLAG_NONE,
        &buffer,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        __uuidof(ID3D12Resource), (void**)UPBuffer) != S_OK)
    {
        return NULL;
    }



    // Describe the data we want to copy into the default buffer.
    D3D12_SUBRESOURCE_DATA subResourceData;
    ZeroMemory(&subResourceData, sizeof(D3D12_SUBRESOURCE_DATA));

    subResourceData.pData = initData;
    subResourceData.RowPitch = (LONG_PTR)size;
    subResourceData.SlicePitch = subResourceData.RowPitch;

    D3D12_RESOURCE_BARRIER barrier;

    barrier = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer,
        D3D12_RESOURCE_STATE_COMMON,
        D3D12_RESOURCE_STATE_COPY_DEST);

    commandList->ResourceBarrier(1, &barrier);

    UpdateSubresources<1>(commandList.Get(), defaultBuffer, *UPBuffer, 0, 0, 1, &subResourceData);
    barrier = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer,
        D3D12_RESOURCE_STATE_COPY_DEST,
        D3D12_RESOURCE_STATE_GENERIC_READ);

    commandList->ResourceBarrier(1, &barrier);

    Execute();
    return defaultBuffer;
}



Microsoft::WRL::ComPtr<ID3D12Device> DirectXDevice::GetDevice()
{
    return device;
}



Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> DirectXDevice::GetCommandList()
{
    return commandList;
}

void rp::DirectXDevice::ResizeSwapChain(HWND hWnd, int width, int height)
{


	//Reset(nullptr,true);
	DXGI_SWAP_CHAIN_DESC1 sd;
	swapChain->GetDesc1(&sd);
	sd.Width = width;
	sd.Height = height;


	ComPtr<IDXGISwapChain1> swapChainLocal;

	ThrowIfFailed(factory->CreateSwapChainForHwnd(
		commandQueue.Get(),        // Swap chain needs the queue so that it can force a flush on it.
		hWnd,
		&sd,
		nullptr,
		nullptr,
		&swapChainLocal
	));

	if (isTearingSupport)
	{
		// When tearing support is enabled we will handle ALT+Enter key presses in the
		// window message loop rather than let DXGI handle it by calling SetFullscreenState.
		factory->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);
	}

	(swapChainLocal->QueryInterface(IID_PPV_ARGS(&swapChain)));

	Execute();

	//IDXGISwapChain1* swapChain1 =  nullptr;
	//swapChain1->QueryInterface(IID_PPV_ARGS(&swapChain));
	//swapChain1->Release();
	//dxgiFactory->Release();
	//


	//swapChain->SetMaximumFrameLatency(2);

}
