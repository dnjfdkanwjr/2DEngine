#include <iostream>
#include "RpGameObject.h"
#include "IMGUI/imgui.h"

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

#ifdef _DEBUG

void rp::RpGameObject::GUIRener()
{
	auto curBuffer = (DirectX::XMFLOAT4X4*)transform.GetConstBuffer().GetDatum();

	ImGui::Begin(this->GetName().c_str());

	ImGui::Text("Transform Value");

	static float position[3]{};

	auto pos = transform.GetPosition();

	ImGui::SliderFloat3("position ", position, -50.f, 50.f);

	transform.SetLocalPosition((DirectX::XMFLOAT3&)position);


	static float rotation[3]{};
	ImGui::SliderFloat3("rotation ", rotation, -50.f, 50.f);

	this->rotation.x = DirectX::XMConvertToRadians(rotation[0]);
	this->rotation.y = DirectX::XMConvertToRadians(rotation[1]);
	this->rotation.z = DirectX::XMConvertToRadians(rotation[2]);
	transform.SetLocalRotation((DirectX::XMFLOAT3&)rotation);

	static float scale[3]{1,1,1};
	ImGui::SliderFloat3("scale ", scale, -50.f, 50.f);

	transform.SetLocalScale((DirectX::XMFLOAT3&)scale);

	//std::copy(&curBuffer->view, &curBuffer->view + sizeof(DirectX::XMFLOAT4X4), viewMatrix);
	ImGui::Text(" ");
	ImGui::Text("ConstBuffer Value");
	ImGui::Text(" ");
	ImGui::Text("localMatrix");
	ImGui::DragFloat4("local 0 ", curBuffer->m[0], -9999.f, 9999.f);
	ImGui::DragFloat4("local 1 ", curBuffer->m[1], -9999.f, 9999.f);
	ImGui::DragFloat4("local 2 ", curBuffer->m[2], -9999.f, 9999.f);
	ImGui::DragFloat4("local 3 ", curBuffer->m[3], -9999.f, 9999.f);


	//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            
	//ImGui::ColorEdit3("clear color", (float*)&clear_color); 

	//if (ImGui::Button("Button")) {

	//}
	//
	//ImGui::SameLine();
	//ImGui::Text("counter = %d", 1);
	ImGui::End();

}

#endif // _DEBUG


