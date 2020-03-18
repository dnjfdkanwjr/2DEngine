#include "Camera.h"
#include "IMGUI/imgui.h"
using namespace rp;
Camera::Camera(float tx, float ty, float tz, float rx, float ry, float rz):
	position(tx,ty,tz),
	rotation(DirectX::XMConvertToRadians(rx), DirectX::XMConvertToRadians(ry), DirectX::XMConvertToRadians(rz))
{


    aspect = FLOAT(1600) / FLOAT(900);  //화면의 가세 세로 비율
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

    DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
    DirectX::XMMATRIX R = DirectX::XMMatrixRotationRollPitchYaw(rotation.x,rotation.y,rotation.z);

    DirectX::XMVECTOR pos = DirectX::XMVectorSet(0, 0, 0, 1); //카메라 로칼 원점
    DirectX::XMVECTOR target = DirectX::XMVectorSet(0, 0, 1, 1); //카메라 로칼 시선
    DirectX::XMVECTOR up = DirectX::XMVectorSet(0, 1, 0, 1); //카메라 로칼 위방향

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
	auto curBuffer = (cb::CameraBuffer*)cbuffer[rp::DirectXDevice::GetCurrentAllocatorIndex()].GetDatum();

	DirectX::XMStoreFloat4x4(&curBuffer->view, DirectX::XMMatrixTranspose(GetView()));
	DirectX::XMStoreFloat4x4(&curBuffer->proj, DirectX::XMMatrixTranspose(GetProjection()));
}

ConstBuffer& rp::Camera::GetConstBuffer() noexcept
{
	return cbuffer[rp::DirectXDevice::GetCurrentAllocatorIndex()];	
}


#ifdef _DEBUG
void rp::Camera::GUIRender()
{
	auto curBuffer = ((cb::CameraBuffer*)(cbuffer[rp::DirectXDevice::GetCurrentAllocatorIndex()].GetDatum()));


	ImGui::Begin("Camera Controller");                          

	ImGui::Text("Transform Value");               

	static float position[3]{};
	ImGui::SliderFloat3("position ", position, -5.f, 5.f);

	this->position.x = position[0];
	this->position.y = position[1];
	this->position.z = position[2];

	static float rotation[3]{};
	ImGui::SliderFloat3("rotation ", rotation, -360.f, 360.f);

	this->rotation.x = DirectX::XMConvertToRadians(rotation[0]);
	this->rotation.y = DirectX::XMConvertToRadians(rotation[1]);
	this->rotation.z = DirectX::XMConvertToRadians(rotation[2]);

	//std::copy(&curBuffer->view, &curBuffer->view + sizeof(DirectX::XMFLOAT4X4), viewMatrix);
	ImGui::Text(" ");
	ImGui::Text("ConstBuffer Value");
	ImGui::Text(" ");
	ImGui::Text("View Matrix Values");
	ImGui::DragFloat4("view 0 ", curBuffer->view.m[0], -9999.f, 9999.f);
	ImGui::DragFloat4("view 1 ", curBuffer->view.m[1], -9999.f, 9999.f);
	ImGui::DragFloat4("view 2 ", curBuffer->view.m[2], -9999.f, 9999.f);
	ImGui::DragFloat4("view 3 ", curBuffer->view.m[3], -9999.f, 9999.f);

	ImGui::Text(" ");
	ImGui::Text("proj Matrix Values");
	ImGui::DragFloat4("proj 0 ", curBuffer->proj.m[0], -9999.f, 9999.f);
	ImGui::DragFloat4("proj 1 ", curBuffer->proj.m[1], -9999.f, 9999.f);
	ImGui::DragFloat4("proj 2 ", curBuffer->proj.m[2], -9999.f, 9999.f);
	ImGui::DragFloat4("proj 3 ", curBuffer->proj.m[3], -9999.f, 9999.f);


	//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            
	//ImGui::ColorEdit3("clear color", (float*)&clear_color); 
	
	//if (ImGui::Button("Button")) {

	//}
	//
	//ImGui::SameLine();
	//ImGui::Text("counter = %d", 1);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::End();
}
#endif // 

