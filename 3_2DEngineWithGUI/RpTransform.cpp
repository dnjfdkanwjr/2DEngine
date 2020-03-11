#include <algorithm>
#include "RpTransform.h"
#include "RpGameObject.h"


rp::RpTransform::RpTransform(RpGameObject* gameObject, RpTransform* parentGameObject, float x, float y, float z, float rx, float ry, float rz, float sx, float sy, float sz) :
	localPosition{x, y, z, 1},
	localRotation{ DirectX::XMConvertToRadians(rx), DirectX::XMConvertToRadians(ry), DirectX::XMConvertToRadians(rz), 1 },
	localScale{sx, sy, sz, 1},
	gameObject{gameObject},
	parent{parentGameObject}
{
	
}

rp::RpTransform::~RpTransform()
{
	Destroy();
}

void rp::RpTransform::DestroyChildrenRecursively(RpTransform* child)
{
	for (int i = 0; i < children.size(); ++i) {
		children[i]->DestroyChildrenRecursively(child);
	}
	delete this->gameObject;
}

void rp::RpTransform::Destroy()
{
	DestroyChildrenRecursively(this);
	if (parent) {
		parent->children.erase(std::remove(children.begin(), children.end(), this),children.end());
	}
}

rp::RpTransform const* rp::RpTransform::GetParent() const noexcept
{
	return parent;
}

void rp::RpTransform::SetParent(RpTransform* parent) noexcept
{
	this->parent = parent;
}

rp::RpGameObject const* rp::RpTransform::GetGameObject() const noexcept
{
	return gameObject;
}

rp::RpTransform const* rp::RpTransform::GetChild(unsigned int index) const noexcept
{
	return children[index];
}

size_t rp::RpTransform::GetChildrenCount() const noexcept
{
	return children.size();
}

const DirectX::XMFLOAT4& rp::RpTransform::GetPosition() const noexcept
{
	return position;
}

const DirectX::XMFLOAT4& rp::RpTransform::GetLocalPosition() const noexcept
{
	return localPosition;
}

const DirectX::XMFLOAT4& rp::RpTransform::GetRotation() const noexcept
{
	return rotation;
}

const DirectX::XMFLOAT4& rp::RpTransform::GetLocalRotation() const noexcept
{
	return localRotation;
}

const DirectX::XMFLOAT4& rp::RpTransform::GetScale() const noexcept
{
	return scale;
}

const DirectX::XMFLOAT4& rp::RpTransform::GetLocalScale() const noexcept
{
	return localScale;
}

const DirectX::XMFLOAT4X4& rp::RpTransform::GetLocalToWorldMatrix() noexcept
{
	if (isDirty) {	
		DirectX::XMMATRIX localMatrix = DirectX::XMMatrixAffineTransformation(
			DirectX::XMLoadFloat4(&scale),
			DirectX::XMQuaternionIdentity(),
			DirectX::XMQuaternionRotationRollPitchYaw(rotation.x,rotation.y,rotation.z),//pitch(x) , yaw(y) , roll(z)
			DirectX::XMLoadFloat4(&position)
			);

		if (parent) {
			auto&& parentWorldMatrix = parent->GetLocalToWorldMatrix();			
			DirectX::XMMATRIX parent= DirectX::XMLoadFloat4x4(&parentWorldMatrix);
			localMatrix = XMMatrixMultiply(parent, localMatrix);
		}

		DirectX::XMStoreFloat4x4(&localToWorld, localMatrix);
	}	
	return localToWorld;
}
