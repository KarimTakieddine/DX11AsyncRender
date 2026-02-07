#include "d3d11_shader_store.h"
#include "platform.h"
#include "renderer.h"

namespace airful_engine
{
	void Renderer::run(HWND window, const GraphicsConfig& config)
	{
		m_runThread = std::thread(&Renderer::execute, this, window, config);
	}

	void Renderer::stop()
	{
		m_shouldRun.store(false, std::memory_order_release);

		if (m_runThread.joinable())
			m_runThread.join();
	}

	void Renderer::initialize(HWND window, const GraphicsConfig& config)
	{
		m_graphicsDevice = std::make_unique<GraphicsDevice>(window, config);

		m_graphicsDevice->setViewport(
			static_cast<float>(config.width),
			static_cast<float>(config.height));

		platformStartTime(m_frameTimer);
	}

	void Renderer::execute(HWND window, const GraphicsConfig& config)
	{
		struct Vertex {
			float x, y, z;
			float r, g, b, a;
		};

		initialize(window, config);

		Vertex vertices[] = {
			{  0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f }, // Top (Red)
			{  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f }, // Right (Green)
			{ -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f }  // Left (Blue)
		};

		Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;

		D3D11_BUFFER_DESC bd = {};
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(vertices);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA initData = {};
		initData.pSysMem = vertices;
		if (m_graphicsDevice->getDevice()->CreateBuffer(&bd, &initData, &vertexBuffer) != S_OK)
			return;

		std::unique_ptr<D3D11VertexShader> vertexShader = std::make_unique<D3D11VertexShader>();
		std::unique_ptr<D3D11PixelShader> pixelShader = std::make_unique<D3D11PixelShader>();
		vertexShader->load(m_graphicsDevice->getDevice().Get(), "./shaders/vertex_color/vertex.cso");
		pixelShader->load(m_graphicsDevice->getDevice().Get(), "./shaders/vertex_color/pixel.cso");

		std::unique_ptr<D3D11ShaderStore> shaderStore = std::make_unique<D3D11ShaderStore>();
		shaderStore->addVertexShader("vertex_color_v", std::move(vertexShader));
		shaderStore->addPixelShader("vertex_color_p", std::move(pixelShader));

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

		D3D11VertexShader* vertexShaderPtr = shaderStore->getVertexShader("vertex_color_v");
		D3D11PixelShader* pixelShaderPtr = shaderStore->getPixelShader("vertex_color_p");

		Microsoft::WRL::ComPtr<ID3DBlob> vertexShaderSource = vertexShaderPtr->getSource();

		Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

		HRESULT hr = m_graphicsDevice->getDevice()->CreateInputLayout(
			layoutDesc,
			ARRAYSIZE(layoutDesc),
			vertexShaderSource->GetBufferPointer(), // Pointer to compiled vertex shader code
			vertexShaderSource->GetBufferSize(),    // Size of compiled vertex shader code
			inputLayout.GetAddressOf()
		);

		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		m_graphicsDevice->getContext()->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
		m_graphicsDevice->getContext()->IASetInputLayout(inputLayout.Get());
		m_graphicsDevice->getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_graphicsDevice->getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_graphicsDevice->getContext()->VSSetShader(vertexShaderPtr->getHandle().Get(), nullptr, 0);
		m_graphicsDevice->getContext()->PSSetShader(pixelShaderPtr->getHandle().Get(), nullptr, 0);

		while (m_shouldRun.load(std::memory_order_acquire))
		{
			float clearColor[] = { 1.0f, 0.71f, 0.76f, 1.0f };

			m_graphicsDevice->targetBackBuffer();

			m_graphicsDevice->getContext()->ClearRenderTargetView(
				m_graphicsDevice->getBackBufferView().Get(),
				clearColor);

			/*
			* Each stride is the size (in bytes) of the elements that are
			* to be used from that vertex buffer.
			*
			* Each offset is the number of bytes between the first element
			* of a vertex buffer and the first element that will be used.
			*/

			//UINT stride = sizeof(Vertex);
			//UINT offset = 0;
			//m_graphicsDevice->getContext()->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
			//m_graphicsDevice->getContext()->IASetInputLayout(inputLayout.Get());
			//m_graphicsDevice->getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			//m_graphicsDevice->getContext()->VSSetShader(vertexShaderPtr->getHandle().Get(), nullptr, 0);
			//m_graphicsDevice->getContext()->PSSetShader(pixelShaderPtr->getHandle().Get(), nullptr, 0);

			m_graphicsDevice->getContext()->Draw(3, 0);

			m_graphicsDevice->getSwapChain()->Present(1, 0);

			DXGI_FRAME_STATISTICS frameStatistics = { };
			if (m_graphicsDevice->getSwapChain()->GetFrameStatistics(&frameStatistics) == S_OK)
				m_frameTimer.end = frameStatistics.SyncQPCTime.QuadPart;

			

			m_frameTimer.start = m_frameTimer.end;
		}
	}
}