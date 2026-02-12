#include "d3d11_render_batch.h"
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

		m_shaderStore = std::make_unique<D3D11ShaderStore>();

		std::unique_ptr<D3D11VertexShader> vertexShader = std::make_unique<D3D11VertexShader>();
		std::unique_ptr<D3D11PixelShader> pixelShader	= std::make_unique<D3D11PixelShader>();

		vertexShader->load(m_graphicsDevice->getDevice().Get(), "./shaders/vertex_transform/vertex.cso");
		pixelShader->load(m_graphicsDevice->getDevice().Get(), "./shaders/vertex_transform/pixel.cso");

		m_shaderStore->addVertexShader("vertex_transform_v", std::move(vertexShader));
		m_shaderStore->addPixelShader("vertex_transform_p", std::move(pixelShader));

		platformStartTime(m_frameTimer);
	}

	void Renderer::execute(HWND window, const GraphicsConfig& config)
	{
		initialize(window, config);

		D3D11RenderBatch renderBatch;
		renderBatch.setGraphicsDevice(m_graphicsDevice.get());
		renderBatch.setVertexShader(m_shaderStore->getVertexShader("vertex_transform_v"));
		renderBatch.setPixelShader(m_shaderStore->getPixelShader("vertex_transform_p"));
		renderBatch.initialize();

		while (m_shouldRun.load(std::memory_order_acquire))
		{
			float clearColor[] = { 1.0f, 0.71f, 0.76f, 1.0f };

			m_graphicsDevice->targetBackBuffer();

			m_graphicsDevice->getContext()->ClearRenderTargetView(
				m_graphicsDevice->getBackBufferView().Get(),
				clearColor);

			renderBatch.use();
			renderBatch.setTransformData(D3D11Transform());

			m_graphicsDevice->getContext()->Draw(3, 0);

			m_graphicsDevice->getSwapChain()->Present(1, 0);

			DXGI_FRAME_STATISTICS frameStatistics = { };
			if (m_graphicsDevice->getSwapChain()->GetFrameStatistics(&frameStatistics) == S_OK)
				m_frameTimer.end = frameStatistics.SyncQPCTime.QuadPart;

			m_frameTimer.start = m_frameTimer.end;
		}
	}
}