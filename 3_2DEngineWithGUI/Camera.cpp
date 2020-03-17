#include "Camera.h"
using namespace rp;
Camera::Camera(float tx, float ty, float tz, float rx, float ry, float rz)
{

    this->tx = tx;
    this->ty = ty;
    this->tz = tz;

    this->rx = DirectX::XMConvertToRadians(rx);
    this->ry = DirectX::XMConvertToRadians(ry);
    this->rz = DirectX::XMConvertToRadians(rz);



    aspect = FLOAT(1600) / FLOAT(900);  //ȭ���� ���� ���� ����
    nearD = FLOAT(0.01);
    farD = FLOAT(1000.0);
    fov = DirectX::XMConvertToRadians(60);

	for (int i = 0; i < konstant::kNumberOfAllocators; ++i) {
		cbuffer.emplace_back(sizeof(cb::CameraBuffer));
	}
}



Camera::~Camera()
{

}


DirectX::XMMATRIX Camera::GetView()
{

    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(tx, ty, tz);
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(rx, ry, rz);

    DirectX::XMVECTOR pos = DirectX::XMVectorSet(0, 0, 0, 1); //ī�޶� ��Į ����
    DirectX::XMVECTOR target = DirectX::XMVectorSet(0, 0, 1, 1); //ī�޶� ��Į �ü�
    DirectX::XMVECTOR up = DirectX::XMVectorSet(0, 1, 0, 1); //ī�޶� ��Į ������

    pos = DirectX::XMVector3Transform(pos, T);
    target = DirectX::XMVector3Transform(target, R);
    up = DirectX::XMVector3Transform(up, R);

    target = DirectX::XMVectorAdd(target, pos);

    return DirectX::XMMatrixLookAtLH(pos, target, up);
}



DirectX::XMMATRIX Camera::GetProjection()
{
    return DirectX::XMMatrixPerspectiveFovLH(fov, aspect, nearD, farD);
}

void rp::Camera::Update()
{
	auto curBuffer = ((cb::CameraBuffer*)(cbuffer[rp::DirectXDevice::GetCurrentAllocatorIndex()].GetDatum()));

	DirectX::XMStoreFloat4x4(&curBuffer->view, DirectX::XMMatrixTranspose(GetView()));
	DirectX::XMStoreFloat4x4(&curBuffer->proj, DirectX::XMMatrixTranspose(GetProjection()));
}

ConstBuffer& rp::Camera::GetConstBuffer() noexcept
{
	return cbuffer[rp::DirectXDevice::GetCurrentAllocatorIndex()];	
}
