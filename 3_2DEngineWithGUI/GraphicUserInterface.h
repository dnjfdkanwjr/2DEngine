#pragma once

#include "DirectXDevice.h"

namespace rp {
	class GraphicUserInterface
	{
	private :
		static Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescHeap;
		static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cachedCommandList;
	public:

		static void Init(HWND hwnd);
		static void PrepareRender();
		static void Render();
		static void Exit();
	};
}

