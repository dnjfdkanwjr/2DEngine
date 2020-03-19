#include <algorithm>
#include "RpTransform.h"
#include "RpGameObject.h"
#include "Configuration.h"
#include "DirectXDevice.h"

rp::RpTransform::RpTransform(RpGameObject* gameObject, RpTransform* parentGameObject, float x, float y, float z, float rx, float ry, float rz, float sx, float sy, float sz) :
	localPosition{x, y, z},
	localRotation{ DirectX::XMConvertToRadians(rx), DirectX::XMConvertToRadians(ry), DirectX::XMConvertToRadians(rz) },
	localScale{sx, sy, sz},
	gameObject{gameObject},
	parent{parentGameObject}
{
	for (int i = 0; i < konstant::kNumberOfAllocators; ++i) {
		cbuffer.emplace_back(sizeof(DirectX::XMFLOAT4X4));
	}
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

const DirectX::XMFLOAT3& rp::RpTransform::GetPosition() const noexcept
{
	return position;
}

const DirectX::XMFLOAT3& rp::RpTransform::GetLocalPosition() const noexcept
{
	return localPosition;
}

const DirectX::XMFLOAT3& rp::RpTransform::GetRotation() const noexcept
{
	return rotation;
}

const DirectX::XMFLOAT3& rp::RpTransform::GetLocalRotation() const noexcept
{
	return localRotation;
}

const DirectX::XMFLOAT3& rp::RpTransform::GetScale() const noexcept
{
	return scale;
}

const DirectX::XMFLOAT3& rp::RpTransform::GetLocalScale() const noexcept
{
	return localScale;
}

const DirectX::XMFLOAT4X4& rp::RpTransform::GetLocalToWorldMatrix() noexcept
{
	//if (isDirty) {	
	//	DirectX::XMMATRIX localMatrix = DirectX::XMMatrixAffineTransformation(
	//		DirectX::XMLoadFloat4(&scale),
	//		DirectX::XMQuaternionIdentity(),
	//		DirectX::XMQuaternionRotationRollPitchYaw(rotation.x,rotation.y,rotation.z),//pitch(x) , yaw(y) , roll(z)
	//		DirectX::XMLoadFloat4(&position)
	//		);

	//	if (parent) {
	//		auto&& parentWorldMatrix = parent->GetLocalToWorldMatrix();			
	//		DirectX::XMMATRIX parent= DirectX::XMLoadFloat4x4(&parentWorldMatrix);
	//		localMatrix = XMMatrixMultiply(parent, localMatrix);
	//	}

	//	DirectX::XMStoreFloat4x4(&localToWorld, localMatrix);
	//}	


	DirectX::XMMATRIX SRT = DirectX::XMMatrixScaling(localScale.x, localScale.y, localScale.z) * 
		DirectX::XMMatrixRotationRollPitchYaw(localRotation.x, localRotation.y, localRotation.z) * 
		DirectX::XMMatrixTranslation(localPosition.x, localPosition.y, localPosition.z);

	DirectX::XMStoreFloat4x4(&localToWorld, (SRT));

	DirectX::XMStoreFloat4x4((DirectX::XMFLOAT4X4*)cbuffer[rp::DirectXDevice::GetCurrentAllocatorIndex()].GetDatum(), DirectX::XMMatrixTranspose(SRT));

	return localToWorld;
}

ID3D12DescriptorHeap* rp::RpTransform::GetDescriptorHeap(int idx) noexcept
{
	return cbuffer[idx].GetDescriptorHeap();
}
ID3D12DescriptorHeap* rp::RpTransform::GetDescriptorHeap() noexcept
{
	return cbuffer[rp::DirectXDevice::GetCurrentAllocatorIndex()].GetDescriptorHeap();
}

rp::ConstBuffer& rp::RpTransform::GetConstBuffer()
{
	return cbuffer[rp::DirectXDevice::GetCurrentAllocatorIndex()];
}

void rp::RpTransform::SetPosition(float x, float y, float z) noexcept
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void rp::RpTransform::SetPosition(const DirectX::XMFLOAT3& position) noexcept
{
	this->position = position;
}

void rp::RpTransform::SetPosition(DirectX::XMFLOAT3&& position) noexcept
{
	this->position = std::move(position);
}

void rp::RpTransform::SetRotation(float x, float y, float z) noexcept
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
}

void rp::RpTransform::SetRotation(const DirectX::XMFLOAT3& rotation) noexcept
{
	this->rotation = rotation;
}

void rp::RpTransform::SetRotation(DirectX::XMFLOAT3&& rotation) noexcept
{
	this->rotation = std::move(rotation);
}



void rp::RpTransform::SetLocalPosition(float x, float y, float z) noexcept
{
	this->localPosition.x = x;
	this->localPosition.y = y;
	this->localPosition.z = z;
}

void rp::RpTransform::SetLocalPosition(const DirectX::XMFLOAT3& position) noexcept
{
	this->localPosition = position;
}

void rp::RpTransform::SetLocalPosition(DirectX::XMFLOAT3&& position) noexcept
{
	this->localPosition = std::move(position);
}

void rp::RpTransform::SetLocalRotation(float x, float y, float z) noexcept
{
	this->localRotation.x = DirectX::XMConvertToRadians(x);
	this->localRotation.y = DirectX::XMConvertToRadians(y);
	this->localRotation.z = DirectX::XMConvertToRadians(z);
}

void rp::RpTransform::SetLocalRotation(const DirectX::XMFLOAT3& rotation) noexcept
{
	this->localRotation = rotation;
}

void rp::RpTransform::SetLocalRotation(DirectX::XMFLOAT3&& rotation) noexcept
{
	this->localRotation = std::move(rotation);
}

void rp::RpTransform::SetLocalScale(float x, float y, float z) noexcept
{
	this->localScale.x = x;
	this->localScale.y = y;
	this->localScale.z = z;
}

void rp::RpTransform::SetLocalScale(const DirectX::XMFLOAT3& scale) noexcept
{
	this->localScale = scale;
}

void rp::RpTransform::SetLocalScale(DirectX::XMFLOAT3&& scale) noexcept
{
	this->localScale = std::move(scale);
}
