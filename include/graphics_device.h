#pragma once

#include <dxgi.h>
#include <d3d11.h>
#include <wrl.h>

#include "graphics_config.h"

using Microsoft::WRL::ComPtr;

namespace airful_engine
{
	class GraphicsDevice
	{
	public:
		explicit GraphicsDevice(HWND window, const GraphicsConfig& config);

		GraphicsDevice(const GraphicsDevice&) = delete;
		GraphicsDevice& operator=(const GraphicsDevice&) = delete;

		ComPtr<ID3D11Device> getDevice() const;
		ComPtr<ID3D11DeviceContext> getContext() const;
		ComPtr<IDXGISwapChain> getSwapChain() const;
		ComPtr<ID3D11RenderTargetView> getBackBufferView() const;

		void setViewport(float width, float height);
		void targetBackBuffer();

	private:
		ComPtr<ID3D11Device> m_device					{ nullptr };
		ComPtr<ID3D11DeviceContext> m_context			{ nullptr };
		ComPtr<IDXGISwapChain> m_swapChain				{ nullptr };
		ComPtr<ID3D11RenderTargetView> m_backBufferView	{ nullptr };
	};
}
