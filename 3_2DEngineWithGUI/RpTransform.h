#pragma once

#include <DirectXMath.h>
#include <vector>
#include "ConstBuffer.h"

namespace rp {
	class RpGameObject;
	class RpTransform
	{

	private:
		DirectX::XMFLOAT3 position{};
		DirectX::XMFLOAT3 rotation{};
		DirectX::XMFLOAT3 scale{};
						
		DirectX::XMFLOAT3 localPosition{};
		DirectX::XMFLOAT3 localRotation{};
		DirectX::XMFLOAT3 localScale{};



		DirectX::XMFLOAT4X4 worldToLocal{};
		DirectX::XMFLOAT4X4 localToWorld{};

		bool isDirty{ true };
		RpGameObject* gameObject{};
		RpTransform* parent{};
		std::vector<RpTransform*> children{};	
		std::vector<ConstBuffer> cbuffer{};

		void DestroyChildrenRecursively(RpTransform* child);
	public:
		void Destroy();
		RpTransform(RpGameObject* gameObject, RpTransform* parentGameObject= nullptr,float x = 0.f,float y = 0.f, float z =0.f,
			float rx = 0.f, float ry = 0.f, float rz = 0.f, 
			float sx = 1.f, float sy = 1.f, float sz = 1.f);
		virtual ~RpTransform();

		void SetParent(RpTransform* parent) noexcept;

		RpGameObject const* GetGameObject() const noexcept;

		RpTransform const* GetChild(unsigned int index) const noexcept;
		size_t GetChildrenCount() const noexcept;

		RpTransform const* GetParent() const noexcept;

		const DirectX::XMFLOAT3& GetPosition() const noexcept;
		const DirectX::XMFLOAT3& GetLocalPosition() const noexcept;
							  
		const DirectX::XMFLOAT3& GetRotation() const noexcept;
		const DirectX::XMFLOAT3& GetLocalRotation() const noexcept;
							  
		const DirectX::XMFLOAT3& GetScale() const noexcept;
		const DirectX::XMFLOAT3& GetLocalScale() const noexcept;

		const DirectX::XMFLOAT4X4& GetLocalToWorldMatrix() noexcept;

		ID3D12DescriptorHeap* GetDescriptorHeap(int idx) noexcept;
		ID3D12DescriptorHeap* GetDescriptorHeap() noexcept;
		ConstBuffer& GetConstBuffer();


		void SetPosition(float x, float y, float z)noexcept;

		void SetPosition(const DirectX::XMFLOAT3& position) noexcept;
		void SetPosition(DirectX::XMFLOAT3&& position)noexcept;


		void SetRotation(float x, float y, float z)noexcept;

		void SetRotation(const DirectX::XMFLOAT3& rotation) noexcept;
		void SetRotation(DirectX::XMFLOAT3&& rotation)noexcept;




		void SetLocalPosition(float x, float y, float z)noexcept;

		void SetLocalPosition(const DirectX::XMFLOAT3& position) noexcept;
		void SetLocalPosition(DirectX::XMFLOAT3&& position)noexcept;


		void SetLocalRotation(float x, float y, float z)noexcept;

		void SetLocalRotation(const DirectX::XMFLOAT3& rotation) noexcept;
		void SetLocalRotation(DirectX::XMFLOAT3&& rotation)noexcept;


		void SetLocalScale(float x, float y, float z)noexcept;

		void SetLocalScale(const DirectX::XMFLOAT3& scale) noexcept;
		void SetLocalScale(DirectX::XMFLOAT3&& scale)noexcept;
	};
}

