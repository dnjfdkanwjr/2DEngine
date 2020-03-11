#pragma once
#include "Base.h"
#include "RpTransform.h"
#include "Component.h"
namespace rp {
	class RpGameObject :
		public Base
	{
	private:
		RpTransform transform;
		std::vector<Component*> components;
	public:
		RpGameObject(float x = 0.f, float y = 0.f, float z = 0.f,
			float rx = 0.f, float ry = 0.f, float rz = 0.f,
			float sx = 0.f, float sy = 0.f, float sz = 0.f,
			RpTransform* parentGameObject = nullptr);
		virtual ~RpGameObject();
		RpTransform& GetTransform() noexcept;


		virtual void Start() = 0;
		virtual void Update() = 0;
	};
}

