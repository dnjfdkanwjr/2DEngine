#pragma once
#include <vector>
#include "ConstBuffer.h"
#include "DirectXDevice.h"
#include "Component.h"
#include "Singleton.h"
#include "Mesh.h"
#include "Camera.h"
namespace rp {


	class IRenderer:
		public Singleton<IRenderer>
	{
	protected:
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pso{};
		Mesh* mesh{};

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cachedCommadList{};



		std::vector<ConstBuffer> cbuffer{};

	public:

		IRenderer();

		virtual ~IRenderer();
		
		void PrepareRender();
		virtual void Draw(RpGameObject* obj,Camera* cam)noexcept;
		void SetMesh(Mesh* mesh) noexcept;

	};




	class RendererWrapper :
		public Component
	{
		IRenderer* renderer{};
	public:
		RendererWrapper(RpGameObject* gameObject,std::string&& name = "Renderer");
		virtual ~RendererWrapper();
		virtual void Start() noexcept override;
		virtual void Update() noexcept override;
	};
}

