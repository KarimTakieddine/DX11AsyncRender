#include <stdexcept>

#include "graphics_device.h"

namespace airful_engine
{
	GraphicsDevice::GraphicsDevice(HWND window, UINT width, UINT height, UINT refreshRate, DXGI_FORMAT format)
	{
		DXGI_SWAP_CHAIN_DESC swapChainDesc = { };

		swapChainDesc.BufferCount	= 2;
		swapChainDesc.SwapEffect	= DXGI_SWAP_EFFECT_FLIP_DISCARD;

		swapChainDesc.BufferDesc = {
			.Width	= width,
			.Height	= height,
			.Format	= format
		};

		swapChainDesc.BufferDesc.RefreshRate = {
			.Numerator		= refreshRate,
			.Denominator	= 1
		};

		swapChainDesc.BufferUsage	= DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow	= window;

		/*
		* The image quality level. The higher the quality, the lower the performance.
		* The valid range is between zero and one less than the level returned by
		* ID3D10Device::CheckMultisampleQualityLevels for Direct3D 10 or
		* ID3D11Device::CheckMultisampleQualityLevels for Direct3D 11
		* 
		* The default sampler mode, with no anti-aliasing, has a count of 1 and a quality level of 0.
		*/

		swapChainDesc.SampleDesc = {
			.Count		= 1,
			.Quality	= 0
		};

		/*
		* Because the target output can't be chosen explicitly when the swap chain is created,
		* we recommend not to create a full-screen swap chain. This can reduce presentation
		* performance if the swap chain size and the output window size do not match.
		* Here are two ways to ensure that the sizes match:
		*
		*	- Create a windowed swap chain and then set it full-screen using
		*	IDXGISwapChain::SetFullscreenState.
		*	- Save a pointer to the swap chain immediately after creation, and
		*	use it to get the output window size during a WM_SIZE event. Then
		*	resize the swap chain buffers (with IDXGISwapChain::ResizeBuffers)
		*	during the transition from windowed to full-screen.
		* If the swap chain is in full-screen mode, before you release it you must use SetFullscreenState to switch it to windowed mode.
		*/

		swapChainDesc.Windowed = TRUE;

		/*
		* If you provide a D3D_FEATURE_LEVEL array that contains D3D_FEATURE_LEVEL_11_1 on a computer
		* that doesn't have the Direct3D 11.1 runtime installed, this function immediately fails
		* with E_INVALIDARG.
		*/

		D3D_FEATURE_LEVEL targetArray[1] = { D3D_FEATURE_LEVEL_11_1 };

		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_1_0_GENERIC;

		HRESULT status = D3D11CreateDeviceAndSwapChain(
			NULL,
			D3D_DRIVER_TYPE_HARDWARE, NULL,
			0,
			targetArray, 1,
			D3D11_SDK_VERSION,
			&swapChainDesc, m_swapChain.GetAddressOf(),
			m_device.GetAddressOf(),
			&featureLevel,
			m_context.GetAddressOf());

		if (status == E_INVALIDARG)
		{
			HRESULT status = D3D11CreateDeviceAndSwapChain(
				NULL,
				D3D_DRIVER_TYPE_HARDWARE, NULL,
				0,
				NULL, 1,
				D3D11_SDK_VERSION,
				&swapChainDesc, m_swapChain.GetAddressOf(),
				m_device.GetAddressOf(),
				&featureLevel,
				m_context.GetAddressOf());
		}

		if (status != S_OK || ( featureLevel != D3D_FEATURE_LEVEL_11_0 && featureLevel != D3D_FEATURE_LEVEL_11_1 ))
		{
			// TODO(Karim): General function for mapping GetLastError() to string
			throw std::runtime_error("Failed to create GraphicsDevice");
		}

		ID3D11Texture2D* pBackBuffer{ nullptr };

		if (m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer) != S_OK)
		{
			// TODO(Karim): General function for mapping GetLastError() to string
			throw std::runtime_error("Failed to get back buffer");
		}

		if (m_device->CreateRenderTargetView(pBackBuffer, NULL, m_backBufferView.GetAddressOf()) != S_OK)
		{
			// TODO(Karim): General function for mapping GetLastError() to string
			throw std::runtime_error("Failed to create render target view");
		}

		/*
		* Bind one or more render targets atomically and the depth-stencil buffer to the
		* output-merger stage.
		*/

		m_context->OMSetRenderTargets(1, m_backBufferView.GetAddressOf(), NULL);
	}
}