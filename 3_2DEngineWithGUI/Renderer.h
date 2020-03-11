#pragma once
#include "Component.h"
#include "Singleton.h"
namespace rp {


	class IRenderer:
		public Singleton<IRenderer>
	{

	public:
		IRenderer(std::string&& vs, std::string&& ps);
		virtual ~IRenderer();

		virtual void Draw();
	};



	class RendererWrapper :
		public Component
	{
		IRenderer* renderer;
	public:
		RendererWrapper(RpGameObject* gameObject,std::string&& name = "Renderer");
		virtual ~RendererWrapper();
		virtual void Start() noexcept override;
		virtual void Update() noexcept override;
	};
}

