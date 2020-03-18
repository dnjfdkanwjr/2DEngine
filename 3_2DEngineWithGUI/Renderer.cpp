#include <iostream>
#include "Renderer.h"
#include "RpGameObject.h"
#include "GraphicResourceMananger.h"
rp::RendererWrapper::RendererWrapper(RpGameObject* gameObject, std::string&& name):
	Component(gameObject, std::move(name))
{

}
rp::RendererWrapper::~RendererWrapper()
{

}

void rp::RendererWrapper::Start() noexcept
{
	
}

void rp::RendererWrapper::Update() noexcept
{
	//renderer->Draw(gameObject);
}




rp::IRenderer::IRenderer()
{
	cachedCommadList = rp::DirectXDevice::GetCommandList();

	if (createConstBuffer(&cbdescHeap, &cbData, sizeof(DirectX::XMFLOAT4X4), &cbUPBuffer) != true)
	{
		std::cout << "cbuffer creation fail" << std::endl;

	}
	else {


	}


}

rp::IRenderer::~IRenderer()
{

}

void rp::IRenderer::PrepareRender()
{
	//rp::DirectXDevice::GetCommandList()->SetGraphicsRootSignature(rp::GraphicResourceMananger::GetRootSignature().Get());
}

void rp::IRenderer::Draw(RpGameObject* obj,Camera* cam) noexcept
{
	cachedCommadList->SetPipelineState(pso.Get());
	cachedCommadList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	cachedCommadList->IASetVertexBuffers(0, 1, &mesh->GetVerticeBufferView());
	cachedCommadList->IASetIndexBuffer(&mesh->GetIndiceBufferView());


	////step A cbuffer setting. < Local Matrix >
	DirectX::XMFLOAT4X4* cbuffer = (DirectX::XMFLOAT4X4*)cbData;
	auto worldViewProj = DirectX::XMLoadFloat4x4(&obj->GetTransform().GetLocalToWorldMatrix()) * cam->GetView() * cam->GetProjection();
	DirectX::XMStoreFloat4x4(cbuffer, DirectX::XMMatrixTranspose(worldViewProj));


	ID3D12DescriptorHeap* descHeap[] = { this->cbdescHeap };
	cachedCommadList->SetDescriptorHeaps(1, descHeap);
	cachedCommadList->SetGraphicsRootDescriptorTable(0, descHeap[0]->GetGPUDescriptorHandleForHeapStart());


	cachedCommadList->DrawIndexedInstanced(mesh->indice.size(), 1, 0, 0, 0);

}

void rp::IRenderer::SetMesh(Mesh* mesh) noexcept
{
	this->mesh = mesh;
}

bool rp::IRenderer::createConstBuffer(ID3D12DescriptorHeap** descHeap, BYTE** Data, int size, ID3D12Resource** UPBuffer)
{

	D3D12_DESCRIPTOR_HEAP_DESC  cbDesc;
	cbDesc.NumDescriptors = 1;
	cbDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbDesc.NodeMask = 0;

	if (rp::DirectXDevice::GetDevice()->CreateDescriptorHeap(&cbDesc,
		__uuidof(ID3D12DescriptorHeap),
		(void**)descHeap) != S_OK)
	{

		return false;

	}

	size = (size + 255) & ~255;

	if (rp::DirectXDevice::GetDevice()->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(size),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		__uuidof(ID3D12Resource), (void**)UPBuffer) != S_OK)

	{
		return false;
	}



	if ((*UPBuffer)->Map(0, NULL, (void**)Data) != S_OK)
	{
		return false;
	}



	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc;
	cbvDesc.BufferLocation = (*UPBuffer)->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = size;
	rp::DirectXDevice::GetDevice()->CreateConstantBufferView(&cbvDesc, (*descHeap)->GetCPUDescriptorHandleForHeapStart());

	return true;

}

