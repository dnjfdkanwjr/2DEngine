#pragma once
#include "GraphicUserInterface.h"
#include "QuadRenderer.h"
#include "Camera.h"
namespace rp {
	class RpFramework
	{

		static QuadRenderer* rendererTest;
		static RpGameObject* testObject;
		static Camera* cam;
	private:
		static void PrepareRender();
		static void Render();
	public:	

		static void Init();
		static void Update();

		static void Exit();
	};
}

