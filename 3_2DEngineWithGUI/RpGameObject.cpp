#include <iostream>
#include "RpGameObject.h"

rp::RpGameObject::RpGameObject(float x, float y, float z,
	float rx, float ry, float rz,
	float sx, float sy, float sz, 
	RpTransform* parentGameObject):
	Base(std::string("GameObject")),
	transform{this,parentGameObject,x,y,z,rx,ry,rz,sx,sy,sz}
{

}

rp::RpGameObject::~RpGameObject()
{
	
}

rp::RpTransform& rp::RpGameObject::GetTransform() noexcept
{
	return transform;
}

