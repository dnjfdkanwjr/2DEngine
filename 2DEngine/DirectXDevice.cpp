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
Microsoft::WRL::ComPtr<ID3D12CommandAllocator>rp::DirectXDevice::commandAllocator{};
Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> rp::DirectXDevice::commandList{};


Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rp::DirectXDevice::rtvHeap{};
Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rp::DirectXDevice::dsvHeap{};
Microsoft::WRL::ComPtr<ID3D12Resource> rp::DirectXDevice::swapChainBuff[konstant::kBackBufferCount]{};
Microsoft::WRL::ComPtr<ID3D12Resource> rp::DirectXDevice::dsBuffer{};

D3D12_VIEWPORT   rp::DirectXDevice::viewRect{};
D3D12_RECT       rp::DirectXDevice::sissorRect{};
int    rp::DirectXDevice::currentBackBuffer{ 0 };
UINT64 rp::DirectXDevice::fenceValue{ 0 };
HANDLE rp::DirectXDevice::fenceEvent{};
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


    fenceEvent = CreateEventEx(NULL, FALSE, FALSE, EVENT_ALL_ACCESS);

    D3D12_COMMAND_QUEUE_DESC queueDesc;
    ZeroMemory(&queueDesc, sizeof(D3D12_COMMAND_QUEUE_DESC));

    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;

    if (device->CreateCommandQueue(&queueDesc, __uuidof(ID3D12CommandQueue), (void**)&commandQueue) != S_OK)
    {
        return false;
    }


    if (device->CreateCommandAllocator(queueDesc.Type, __uuidof(ID3D12CommandAllocator), (void**)&commandAllocator) != S_OK)
    {
        return false;

    }


    if (device->CreateCommandList(0, queueDesc.Type, commandAllocator.Get(), NULL, __uuidof(ID3D12GraphicsCommandList), (void**)&commandList) != S_OK)
    {

        return false;
    }

    commandList->Close();

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



    D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
    device->CreateDepthStencilView(dsBuffer.Get(), NULL, dsvHandle); 


    D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(dsBuffer.Get(),
        D3D12_RESOURCE_STATE_COMMON,
        D3D12_RESOURCE_STATE_DEPTH_WRITE);
    commandList->ResourceBarrier(1, &barrier);



    viewRect = { 0.0f, 0.0f, (float)width, float(height), 0.0f, 1.0f };
    sissorRect = { 0, 0,width, height };


    return true;
}



void  DirectXDevice::Clear(unsigned char r, unsigned char g, unsigned char b)
{

    Reset(NULL, true);

    D3D12_RESOURCE_BARRIER barrier;

    barrier = CD3DX12_RESOURCE_BARRIER::Transition(swapChainBuff[currentBackBuffer].Get(),
        D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET);

    commandList->ResourceBarrier(1, &barrier);



    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
    D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();

    unsigned int incSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    rtvHandle.ptr += incSize * currentBackBuffer;


    static float BGColor[4];
    BGColor[0] = r / 255.0f;
    BGColor[1] = g / 255.0f;
    BGColor[2] = b / 255.0f;
    BGColor[3] = 1.0f;


    commandList->ClearRenderTargetView(rtvHandle, BGColor, 0, NULL);
    commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, NULL);


    Execute(true);
}



void DirectXDevice::Render()
{

    Reset(NULL, true);

    D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(swapChainBuff[currentBackBuffer].Get(),
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PRESENT);

    commandList->ResourceBarrier(1, &barrier);

    Execute(true);

    swapChain->Present(0, 0);
	currentBackBuffer = swapChain->GetCurrentBackBufferIndex();
}



void DirectXDevice::Reset(ID3D12PipelineState* pipelineState, bool alloc, bool view)
{
    if (alloc == true)
    {
        if (commandAllocator->Reset() != S_OK)
        {

        }
    }


    if (commandList->Reset(commandAllocator.Get(), pipelineState) != S_OK)
    {

    }

    if (view == true)
    {

        D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
        D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();

        unsigned int incSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        rtvHandle.ptr += incSize * currentBackBuffer;


        commandList->RSSetViewports(1, &viewRect);
        commandList->RSSetScissorRects(1, &sissorRect);

        commandList->OMSetRenderTargets(1, &rtvHandle, true, &dsvHandle);
    }

}



void DirectXDevice::Execute(bool flush)
{
    commandList->Close();

    ID3D12CommandList* cmdLists[1] = { commandList.Get()};

    commandQueue->ExecuteCommandLists(1, cmdLists);

    if (flush == true)
    {
        fenceValue++;

        if (commandQueue->Signal(fence.Get(), fenceValue) == S_OK)
        {
            while(fence->GetCompletedValue() < fenceValue)
            {
                _YIELD_PROCESSOR();
            }
        }
    }
}






Microsoft::WRL::ComPtr<ID3D12Resource> DirectXDevice::CreateBuffer(void* initData, UINT64 size, ID3D12Resource** UPBuffer)
{

    Reset(NULL, true);

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

    Execute(true);
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