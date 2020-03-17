#pragma once
#include <d3d12.h>
#include <wrl.h>
namespace rp {
	class ConstBuffer
	{
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> cbdescHeap;
		Microsoft::WRL::ComPtr<ID3D12Resource> cbUPBuffer;

		BYTE* cbData;

		int size;
	public:
		ConstBuffer(int size);
		~ConstBuffer();

		ID3D12DescriptorHeap* GetDescriptorHeap()noexcept;
		ID3D12Resource* GetUploadBuffer()noexcept;

		BYTE* GetDatum() noexcept;
	};
}

