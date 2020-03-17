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
	renderer->Draw(gameObject);
}




rp::IRenderer::IRenderer()
{
	cachedCommadList = rp::DirectXDevice::GetCommandList();
}

rp::IRenderer::~IRenderer()
{

}

void rp::IRenderer::PrepareRender()
{
	//rp::DirectXDevice::GetCommandList()->SetGraphicsRootSignature(rp::GraphicResourceMananger::GetRootSignature().Get());
}

void rp::IRenderer::Draw(RpGameObject* obj) noexcept
{
	
	cachedCommadList->SetPipelineState(pso.Get());
	cachedCommadList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//if (mesh == nullptr) return;
	cachedCommadList->IASetVertexBuffers(0, 1, &mesh->GetVerticeBufferView());
	cachedCommadList->IASetIndexBuffer(&mesh->GetIndiceBufferView());


	//step A cbuffer setting. < Local Matrix >
	obj->GetTransform().GetLocalToWorldMatrix();
	//step B cbuffer material data setting < Texture Number >
	cachedCommadList->SetGraphicsRootConstantBufferView(0,obj->GetTransform().GetConstBuffer().GetUploadBuffer()->GetGPUVirtualAddress());

	cachedCommadList->DrawIndexedInstanced(mesh->indice.size(), 1, 0, 0, 0);
}

void rp::IRenderer::SetMesh(Mesh* mesh) noexcept
{
	this->mesh = mesh;
}

