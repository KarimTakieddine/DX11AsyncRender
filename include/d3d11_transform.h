#pragma once

#include <type_traits>

#include <DirectXMath.h>

namespace airful_engine
{
	class D3D11Transform
	{
	public:
		enum class DirtyFlags : uint64_t
		{
			NONE	= 0,
			DIRTY	= 1
		};

		DirectX::XMFLOAT3 getLocalScale() const;
		void setLocalScale(const DirectX::XMFLOAT3& scale);

		DirectX::XMFLOAT3 getPosition() const;
		void setPosition(const DirectX::XMFLOAT3& position);

		const DirectX::XMMATRIX& getModelMatrix() const;

		void translate(const DirectX::XMFLOAT3& displacement);

		// TODO(Karim): Use quaternions to fix gimbal lock!

		void rotateX(float degrees);

		void rotateY(float degrees);

		void rotateZ(float degrees);

		void recalculateModelMatrix();

	private:
		DirectX::XMMATRIX m_scaleMatrix			{ DirectX::XMMatrixIdentity() };
		DirectX::XMMATRIX m_rotationMatrix		{ DirectX::XMMatrixIdentity() };
		DirectX::XMMATRIX m_translationMatrix	{ DirectX::XMMatrixIdentity() };
		DirectX::XMMATRIX m_modelMatrix			{ DirectX::XMMatrixIdentity() };
		DirtyFlags m_dirtyFlags					{ DirtyFlags::NONE };
	};

	using DirtyFlags = D3D11Transform::DirtyFlags;

	inline DirtyFlags operator&(DirtyFlags l, DirtyFlags r)
	{
		return static_cast<DirtyFlags>(
			static_cast<std::underlying_type_t<DirtyFlags>>(l) &
			static_cast<std::underlying_type_t<DirtyFlags>>(r));
	}

	inline DirtyFlags& operator&=(DirtyFlags& l, DirtyFlags r)
	{
		return l = l & r;
	}

	inline DirtyFlags operator|(DirtyFlags l, DirtyFlags r)
	{
		return static_cast<DirtyFlags>(
			static_cast<std::underlying_type_t<DirtyFlags>>(l) |
			static_cast<std::underlying_type_t<DirtyFlags>>(r));
	}

	inline DirtyFlags& operator|=(DirtyFlags& l, DirtyFlags r)
	{
		return l = l | r;
	}

	inline DirtyFlags operator~(DirtyFlags f)
	{
		return static_cast<DirtyFlags>(~(static_cast<std::underlying_type_t<DirtyFlags>>(f)));
	}
}
