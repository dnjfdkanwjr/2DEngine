#pragma once

#include "DirectXDevice.h"
namespace rp {
	class GraphicResourceMananger
	{
	private:
		static std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3DBlob>> shaders;
		static Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;

	private:
		static std::array<const CD3DX12_STATIC_SAMPLER_DESC, 7> GetStaticSamplers();
		static void MakeRootSignature();
		static void MakeShaders();
	public:

		static void Init();
		static Microsoft::WRL::ComPtr<ID3D12RootSignature> GetRootSignature();
		static std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3DBlob>>& GetShaders();
	};
}

