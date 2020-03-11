#include "Component.h"

rp::Component::Component(RpGameObject* object ,std::string&& name):
	Base{std::move(name)},
	gameObject{object}
{
}

rp::Component::~Component()
{
}
