#pragma once
#include "GraphicUserInterface.h"
namespace rp {
	class RpFramework
	{
	private:
		static void PrepareRender();
		static void Render();
	public:	

		static void Init();
		static void Update();

		static void Exit();
	};
}

