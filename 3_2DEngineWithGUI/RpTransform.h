#pragma once

#include <DirectXMath.h>
#include <vector>
namespace rp {
	class RpGameObject;
	class RpTransform
	{

	private:
		DirectX::XMFLOAT4 position{};
		DirectX::XMFLOAT4 rotation{};
		DirectX::XMFLOAT4 scale{};

		DirectX::XMFLOAT4 localPosition{};
		DirectX::XMFLOAT4 localRotation{};
		DirectX::XMFLOAT4 localScale{};



		DirectX::XMFLOAT4X4 worldToLocal{};
		DirectX::XMFLOAT4X4 localToWorld{};

		bool isDirty{ true };
		RpGameObject* gameObject{};
		RpTransform* parent{};
		std::vector<RpTransform*> children{};	
		 


		void DestroyChildrenRecursively(RpTransform* child);
	public:
		void Destroy();
		RpTransform(RpGameObject* gameObject, RpTransform* parentGameObject= nullptr,float x = 0.f,float y = 0.f, float z =0.f,
			float rx = 0.f, float ry = 0.f, float rz = 0.f, 
			float sx = 0.f, float sy = 0.f, float sz = 0.f);
		virtual ~RpTransform();

		void SetParent(RpTransform* parent) noexcept;

		RpGameObject const* GetGameObject() const noexcept;

		RpTransform const* GetChild(unsigned int index) const noexcept;
		size_t GetChildrenCount() const noexcept;

		RpTransform const* GetParent() const noexcept;

		const DirectX::XMFLOAT4& GetPosition() const noexcept;
		const DirectX::XMFLOAT4& GetLocalPosition() const noexcept;

		const DirectX::XMFLOAT4& GetRotation() const noexcept;
		const DirectX::XMFLOAT4& GetLocalRotation() const noexcept;

		const DirectX::XMFLOAT4& GetScale() const noexcept;
		const DirectX::XMFLOAT4& GetLocalScale() const noexcept;

		const DirectX::XMFLOAT4X4& GetLocalToWorldMatrix() noexcept;

	};
}

