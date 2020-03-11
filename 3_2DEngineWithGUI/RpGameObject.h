#pragma once
#include "Base.h"
#include "RpTransform.h"
namespace rp {
	class RpGameObject :
		public Base
	{
	private:
		RpTransform transform;

	public:
		RpGameObject(float x = 0.f, float y = 0.f, float z = 0.f,
			float rx = 0.f, float ry = 0.f, float rz = 0.f,
			float sx = 0.f, float sy = 0.f, float sz = 0.f,
			RpTransform* parentGameObject = nullptr);
		virtual ~RpGameObject();
		RpTransform& GetTransform() noexcept;
	};
}

