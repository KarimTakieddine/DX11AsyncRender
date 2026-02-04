#pragma once

#include <dxgi.h>
#include <d3d11.h>
#include <wrl.h>

using Microsoft::WRL::ComPtr;

namespace airful_engine
{
	class GraphicsDevice
	{
	public:
		explicit GraphicsDevice(
			HWND window,
			UINT width,
			UINT height,
			UINT refreshRate,
			DXGI_FORMAT format);

		GraphicsDevice(const GraphicsDevice&) = delete;
		GraphicsDevice& operator=(const GraphicsDevice&) = delete;

	private:
		ComPtr<ID3D11Device> m_device					{ nullptr };
		ComPtr<ID3D11DeviceContext> m_context			{ nullptr };
		ComPtr<IDXGISwapChain> m_swapChain				{ nullptr };
		ComPtr<ID3D11RenderTargetView> m_backBufferView	{ nullptr };
	};
}
