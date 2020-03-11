#pragma once
#include "Base.h"
namespace rp {
	class RpGameObject;
	class Component :
		public rp::Base
	{
		RpGameObject* gameObject;
	public:
		Component(RpGameObject* gameObject,std::string&& name);
		virtual ~Component();

		virtual void Update() noexcept = 0;
		virtual void Start() noexcept = 0;

	};
}

