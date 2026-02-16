#include "d3d11_transform.h"

namespace dx11_async_render
{
	DirectX::XMFLOAT3 D3D11Transform::getLocalScale() const
	{
		return {
			DirectX::XMVectorGetX(m_scaleMatrix.r[0]),
			DirectX::XMVectorGetY(m_scaleMatrix.r[0]),
			DirectX::XMVectorGetZ(m_scaleMatrix.r[0])
		};
	}

	void D3D11Transform::setLocalScale(const DirectX::XMFLOAT3& scale)
	{
		m_scaleMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

		m_dirtyFlags |= DirtyFlags::DIRTY;
	}

	DirectX::XMFLOAT3 D3D11Transform::getPosition() const
	{
		DirectX::XMFLOAT3 position;

		DirectX::XMStoreFloat3(&position, m_modelMatrix.r[3]);

		return DirectX::XMFLOAT3();
	}

	void D3D11Transform::setPosition(const DirectX::XMFLOAT3& position)
	{
		DirectX::XMVECTOR xmPos = DirectX::XMVectorSet(position.x, position.y, position.z, 1.0f);

		m_modelMatrix.r[3] = xmPos;
	}

	const DirectX::XMMATRIX& D3D11Transform::getModelMatrix() const
	{
		return m_modelMatrix;
	}

	void D3D11Transform::translate(const DirectX::XMFLOAT3& displacement)
	{
		m_translationMatrix = DirectX::XMMatrixTranslation(displacement.x, displacement.y, displacement.z);
	
		m_dirtyFlags |= DirtyFlags::DIRTY;
	}

	void D3D11Transform::rotateX(float degrees)
	{

	}

	void D3D11Transform::rotateY(float degrees)
	{

	}

	void D3D11Transform::rotateZ(float degrees)
	{

	}

	void D3D11Transform::recalculateModelMatrix()
	{
		if ((m_dirtyFlags & DirtyFlags::DIRTY) != DirtyFlags::DIRTY)
			return;

		m_modelMatrix = m_scaleMatrix * m_rotationMatrix * m_translationMatrix;

		m_dirtyFlags &= ~DirtyFlags::DIRTY;
	}
}