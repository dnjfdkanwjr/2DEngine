#pragma once
#include "DirectXDevice.h"
#include "Renderer.h"

namespace rp {

	class QuadRenderer :
		public IRenderer
	{
		
	public:
		QuadRenderer();
		virtual~QuadRenderer();
	};
}

