#pragma once
#include "DirectXDevice.h"
#include "Component.h"
#include "Singleton.h"
namespace rp {


	class IRenderer:
		public Singleton<IRenderer>
	{
	protected:
		Microsoft::WRL::ComPtr<ID3D12PipelineState> pso{};
	public:

		IRenderer();

		virtual ~IRenderer();
		virtual void Draw();
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

