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

	for (int i = 0; i < konstant::kNumberOfAllocators; ++i) {
		cbuffer.emplace_back(sizeof(cb::ObjectBuffer));
	}


}

rp::IRenderer::~IRenderer()
{

}

void rp::IRenderer::PrepareRender()
{
	//rp::DirectXDevice::GetCommandList()->SetGraphicsRootSignature(rp::GraphicResourceMananger::GetRootSignature().Get());
}

void rp::IRenderer::Draw(RpGameObject* obj, Camera* cam) noexcept
{
	cachedCommadList->SetPipelineState(pso.Get());
	cachedCommadList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	cachedCommadList->IASetVertexBuffers(0, 1, &mesh->GetVerticeBufferView());
	cachedCommadList->IASetIndexBuffer(&mesh->GetIndiceBufferView());


	////step A cbuffer setting. < Local Matrix >
	cb::ObjectBuffer* cbuffer = (cb::ObjectBuffer*)obj->GetTransform().GetConstBuffer().GetDatum();

	auto worldViewProj = DirectX::XMLoadFloat4x4(&obj->GetTransform().GetLocalToWorldMatrix()) * cam->GetView() * cam->GetProjection();
	DirectX::XMStoreFloat4x4(&cbuffer->worldViewProj, DirectX::XMMatrixTranspose(worldViewProj));


	ID3D12DescriptorHeap* descHeap[] = { obj->GetTransform().GetConstBuffer().GetDescriptorHeap() };
	cachedCommadList->SetDescriptorHeaps(1, descHeap);
	cachedCommadList->SetGraphicsRootDescriptorTable(0, descHeap[0]->GetGPUDescriptorHandleForHeapStart());

	auto& subMeshLocalCache = mesh->subMeshes;

	for (const auto& subMesh : subMeshLocalCache) {
		cachedCommadList->DrawIndexedInstanced(
			subMesh.lastIndex - subMesh.startIndex,
			1,
			subMesh.startIndex,
			subMesh.startVertex,
			0);
	}
}

void rp::IRenderer::SetMesh(Mesh* mesh) noexcept
{
	this->mesh = mesh;
}
