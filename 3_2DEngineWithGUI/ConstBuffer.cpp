#include <iostream>
#include "ConstBuffer.h"
#include "DirectXDevice.h"

rp::ConstBuffer::ConstBuffer(int size)
{
	D3D12_DESCRIPTOR_HEAP_DESC  cbDesc;
	cbDesc.NumDescriptors = 1;
	cbDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbDesc.NodeMask = 0;

	if (rp::DirectXDevice::GetDevice()->CreateDescriptorHeap(&cbDesc,
		__uuidof(ID3D12DescriptorHeap),
		(void**)cbdescHeap.GetAddressOf()) != S_OK)
	{
	
	}

	size = (size + 255) & ~255;
	this->size = size;
	if (rp::DirectXDevice::GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(size),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		__uuidof(ID3D12Resource), (void**)cbUPBuffer.GetAddressOf()) != S_OK)

	{

	}

	if (cbUPBuffer->Map(0, nullptr, (void**)&cbData) != S_OK)
	{
		
	}

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	cbvDesc.BufferLocation = cbUPBuffer->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = size;
	rp::DirectXDevice::GetDevice()->CreateConstantBufferView(&cbvDesc, (cbdescHeap)->GetCPUDescriptorHandleForHeapStart());

}

rp::ConstBuffer::~ConstBuffer()
{

}

ID3D12DescriptorHeap* rp::ConstBuffer::GetDescriptorHeap() noexcept
{
	return cbdescHeap.Get();
}

ID3D12Resource* rp::ConstBuffer::GetUploadBuffer() noexcept
{
	return cbUPBuffer.Get();
}

BYTE* rp::ConstBuffer::GetDatum() noexcept
{
	return cbData;
}
