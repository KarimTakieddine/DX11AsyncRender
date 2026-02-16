#pragma once

#include <d3d11.h>
#include <wrl.h>

#include "d3d11_transform.h"

namespace dx11_async_render
{
	class GraphicsDevice;
	class D3D11VertexShader;
	class D3D11PixelShader;

	class D3D11RenderBatch
	{
	public:
		void setGraphicsDevice(GraphicsDevice* device);
		void setVertexShader(D3D11VertexShader* shader);
		void setPixelShader(D3D11PixelShader* shader);

		bool initialize();
		bool setTransformData(const D3D11Transform& transform);
		bool use();

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer		{ nullptr };
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_transformBuffer	{ nullptr };
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout	{ nullptr };
		GraphicsDevice* m_graphicsDevice						{ nullptr };
		D3D11VertexShader* m_vertexShader						{ nullptr };
		D3D11PixelShader* m_pixelShader							{ nullptr };
	};
}
