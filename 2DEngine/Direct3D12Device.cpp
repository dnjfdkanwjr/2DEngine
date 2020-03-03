#include "Direct3D12Device.h"
#include "DirectXHelper.h"

bool CheckTearingSupport();
void GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter);

void rp::Direct3D12Device::Init(HWND handleWindow, int width, int height)
{
	using Microsoft::WRL::ComPtr;
	UINT dxgiFactoryFlags = 0;
#if defined(_DEBUG)
	// Enable the debug layer (requires the Graphics Tools "optional feature").
	// NOTE: Enabling the debug layer after device creation will invalidate the active device.
	{
		Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();

			// Enable additional debug layers.
			dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		}
	}
#endif

	Microsoft::WRL::ComPtr<IDXGIFactory4> factory;
	ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)));

	ComPtr<IDXGIAdapter1> hardwareAdapter;
	GetHardwareAdapter(factory.Get(), &hardwareAdapter);

	ThrowIfFailed(D3D12CreateDevice(
		hardwareAdapter.Get(),
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(&this->device)
	));

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	ThrowIfFailed(this->device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&this->commandQueue)));
	NAME_D3D12_OBJECT(this->commandQueue);


	bool tearing = CheckTearingSupport();
	
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = constant::maxFrameCount;
	swapChainDesc.Width = width;
	swapChainDesc.Height = height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Flags = tearing ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;


	ComPtr<IDXGISwapChain1> swapChain;
	ThrowIfFailed(factory->CreateSwapChainForHwnd(
		this->commandQueue.Get(),        // Swap chain needs the queue so that it can force a flush on it.
		handleWindow,
		&swapChainDesc,
		nullptr,
		nullptr,
		&swapChain
	));



	if (tearing) {
		factory->MakeWindowAssociation(handleWindow, DXGI_MWA_NO_ALT_ENTER);
	}
	ThrowIfFailed(swapChain.As(& this->swapChain ));
	this->currentFrameIndex = this->swapChain->GetCurrentBackBufferIndex();


	// Create descriptor heaps.
	{
		// Describe and create a render target view (RTV) descriptor heap.
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = constant::maxFrameCount; // + 1 for the intermediate render target.
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		ThrowIfFailed(this->device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&this->renderTargetViewHeap)));



		global::rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		global::cbvSrvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}

	// Create command allocators for each frame.
	for (UINT n = 0; n < constant::maxFrameCount ; n++)
	{
		ThrowIfFailed(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandLists[n]->commandAllocator)));
	}
}

void GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter)
{
	ComPtr<IDXGIAdapter1> adapter;
	*ppAdapter = nullptr;

	for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
	{
		DXGI_ADAPTER_DESC1 desc;
		ThrowIfFailed(adapter->GetDesc1(&desc));

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			// Don't select the Basic Render Driver adapter.
			// If you want a software adapter, pass in "/warp" on the command line.
			continue;
		}

		// Check to see if the adapter supports Direct3D 12, but don't create the
		// actual device yet.
		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
		{
			break;
		}
	}

	*ppAdapter = adapter.Detach();
}

bool CheckTearingSupport()
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
