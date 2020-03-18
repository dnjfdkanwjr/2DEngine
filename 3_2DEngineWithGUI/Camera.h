#pragma once
#include <DirectXMath.h>
#include "ConstBuffer.h"
#include "DirectXDevice.h"

namespace rp {
	class Camera
	{
	private:

		float fov;
		float aspect;
		float nearD;
		float farD;

		DirectX::XMFLOAT3 position{};
		DirectX::XMFLOAT3 rotation{};
		
		std::vector<ConstBuffer> cbuffer{};

	public:


		Camera(float tx = 0.0f, float ty = 0.0f, float tz = -2.0f, float rx = 0.0f, float ry = 0.0f, float rz = 0.0f);
		~Camera();



		DirectX::XMMATRIX GetView();
		DirectX::XMMATRIX GetProjection();

		void Update();

		ConstBuffer& GetConstBuffer()noexcept;
#ifdef _DEBUG
		void GUIRender();
#endif
	};
}