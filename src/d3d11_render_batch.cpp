#include "d3d11_render_batch.h"
#include "d3d11_shader.h"
#include "d3d11_vertex.h"
#include "graphics_device.h"

namespace
{
	struct TransformBuffer
	{
		DirectX::XMMATRIX modelMatrix{ DirectX::XMMatrixIdentity() };
	};
}

namespace dx11_async_render
{
	void D3D11RenderBatch::setGraphicsDevice(GraphicsDevice* device)
	{
		m_graphicsDevice = device;
	}

	void D3D11RenderBatch::setVertexShader(D3D11VertexShader* shader)
	{
		m_vertexShader = shader;
	}

	void D3D11RenderBatch::setPixelShader(D3D11PixelShader* shader)
	{
		m_pixelShader = shader;
	}

	bool D3D11RenderBatch::initialize()
	{
		if (!m_graphicsDevice || !m_vertexShader || !m_pixelShader)
			return false;

		D3D11Vertex vertices[] = {
			{ { 0.0f,  0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },	// Top (Red)
			{ {  0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } }, // Right (Green)
			{ { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f} }	// Left (Blue)
		};

		D3D11_BUFFER_DESC vertexBufferDesc = {
			.ByteWidth	= sizeof(vertices),
			.Usage		= D3D11_USAGE_IMMUTABLE,
			.BindFlags	= D3D11_BIND_VERTEX_BUFFER
		};

		D3D11_SUBRESOURCE_DATA vertexBufferData = {
			.pSysMem = vertices
		};

		if (m_graphicsDevice->getDevice()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &m_vertexBuffer) != S_OK)
			return false;

		D3D11_INPUT_ELEMENT_DESC layoutDesc[] = {
			{
				"POSITION", 0,						// Semantic Name and Index
				DXGI_FORMAT_R32G32B32_FLOAT,		// Format (3 floats)
				0, 0,                               // Input Slot and Aligned Byte Offset
				D3D11_INPUT_PER_VERTEX_DATA, 0		// Input Class and Step Rate
			},
			{
				"COLOR", 0,							// Semantic Name and Index
				DXGI_FORMAT_R32G32B32A32_FLOAT,     // Format (4 floats)
				0, D3D11_APPEND_ALIGNED_ELEMENT,	// Offset automatically follows Position
				D3D11_INPUT_PER_VERTEX_DATA, 0
			}
		};

		Microsoft::WRL::ComPtr<ID3DBlob> vertexShaderSource = m_vertexShader->getSource();

		if (m_graphicsDevice->getDevice()->CreateInputLayout(
			layoutDesc,
			ARRAYSIZE(layoutDesc),
			vertexShaderSource->GetBufferPointer(), // Pointer to compiled vertex shader code
			vertexShaderSource->GetBufferSize(),    // Size of compiled vertex shader code
			&m_inputLayout
		) != S_OK)
			return false;

		D3D11_BUFFER_DESC transformBufferDesc = {
			.ByteWidth				= sizeof(TransformBuffer),
			.Usage					= D3D11_USAGE_DYNAMIC,
			.BindFlags				= D3D11_BIND_CONSTANT_BUFFER,
			.CPUAccessFlags			= D3D11_CPU_ACCESS_WRITE,
			.MiscFlags				= 0,
			.StructureByteStride	= 0
		};

		if (m_graphicsDevice->getDevice()->CreateBuffer(&transformBufferDesc, nullptr, &m_transformBuffer) != S_OK)
			return false;
	
		return true;
	}

	bool D3D11RenderBatch::setTransformData(const D3D11Transform& transform)
	{
		if (!m_graphicsDevice || !m_transformBuffer)
			return false;

		D3D11_MAPPED_SUBRESOURCE mappedResource = { };

		if (m_graphicsDevice->getContext()->Map(m_transformBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource) != S_OK)
			return false;

		TransformBuffer* mappedData = (TransformBuffer*)mappedResource.pData;
		mappedData->modelMatrix		= transform.getModelMatrix();

		m_graphicsDevice->getContext()->Unmap(m_transformBuffer.Get(), 0);

		return true;
	}

	bool D3D11RenderBatch::use()
	{
		if (!m_vertexBuffer || !m_inputLayout)
			return false;

		/*
		* Each stride is the size (in bytes) of the elements that are
		* to be used from that vertex buffer.
		*
		* Each offset is the number of bytes between the first element
		* of a vertex buffer and the first element that will be used.
		*/

		UINT stride = sizeof(D3D11Vertex);
		UINT offset = 0;

		m_graphicsDevice->getContext()->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
		m_graphicsDevice->getContext()->IASetInputLayout(m_inputLayout.Get());
		m_graphicsDevice->getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_graphicsDevice->getContext()->VSSetShader(m_vertexShader->getHandle().Get(), nullptr, 0);
		m_graphicsDevice->getContext()->PSSetShader(m_pixelShader->getHandle().Get(), nullptr, 0);
		m_graphicsDevice->getContext()->VSSetConstantBuffers(0, 1, m_transformBuffer.GetAddressOf());

		return true;
	}
}