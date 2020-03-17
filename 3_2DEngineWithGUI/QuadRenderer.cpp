#include <iostream>
#include "QuadRenderer.h"
#include "GraphicResourceMananger.h"


rp::QuadRenderer::QuadRenderer():
	IRenderer()
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psDesc;
	ZeroMemory(&psDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));

	psDesc.InputLayout = { rp::inputlayout::kInputLayout, 4 };
	psDesc.pRootSignature = GraphicResourceMananger::GetRootSignature().Get();

	auto shaders = GraphicResourceMananger::GetShaders();

	auto vs = shaders["QuadVS"];
	auto ps = shaders["QuadPS"];

	psDesc.VS = { vs->GetBufferPointer() , vs->GetBufferSize() };
	psDesc.PS = { ps->GetBufferPointer() , ps->GetBufferSize() };

	psDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);

	psDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

	D3D12_RENDER_TARGET_BLEND_DESC transparencyBlendDesc;
	transparencyBlendDesc.BlendEnable = true;
	transparencyBlendDesc.LogicOpEnable = false;
	transparencyBlendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	transparencyBlendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	transparencyBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
	transparencyBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	transparencyBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	transparencyBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	transparencyBlendDesc.LogicOp = D3D12_LOGIC_OP_NOOP;
	transparencyBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	psDesc.BlendState.RenderTarget[0] = transparencyBlendDesc;
	psDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psDesc.SampleMask = UINT_MAX;
	psDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psDesc.NumRenderTargets = 1;
	psDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psDesc.SampleDesc.Count = 1;
	psDesc.SampleDesc.Quality = 0;
	psDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	psDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;

	if (rp::DirectXDevice::GetDevice()->CreateGraphicsPipelineState(&psDesc,
		__uuidof(ID3D12PipelineState),
		(void**)&pso) != S_OK)

	{
		std::cout << "pso creation fail [quad renderer]" << std::endl;
	}
}


rp::QuadRenderer::~QuadRenderer()
{
}
