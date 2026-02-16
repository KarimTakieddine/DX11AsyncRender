#include "d3d11_render_batch.h"
#include "platform.h"
#include "render_command_buffer.h"
#include "renderer.h"
#include "win32_window.h"

namespace dx11_async_render
{
	void Renderer::run(Win32Window* window, RenderCommandBuffer* commandBuffer, const GraphicsConfig& config)
	{
		m_window		= window;
		m_commandBuffer = commandBuffer;
		m_runThread		= std::thread(&Renderer::execute, this, window->getHandle(), config);
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

		D3D11Transform transform;
		DirectX::XMFLOAT3 displacement{ 0.0f, 0.0f, 0.0f };

		while (m_shouldRun.load(std::memory_order_acquire))
		{
			Win32Window::Message windowMessage = { };
			while (m_window->dequeMessage(windowMessage))
			{
				switch (windowMessage.type)
				{
				case Win32Window::MessageType::RESIZE:
				{
					UINT width	= static_cast<UINT>( (windowMessage.data & 0xFFFFFFFF00000000) >> 32 );
					UINT height	= static_cast<UINT>( (windowMessage.data & 0xFFFFFFFF) );

					m_graphicsDevice->resizeBuffers(width, height);
					m_graphicsDevice->setViewport(static_cast<float>(width), static_cast<float>(height));

					break;
				}
				default:
					break;
				}
			}

			float clearColor[] = { 1.0f, 0.71f, 0.76f, 1.0f };

			m_graphicsDevice->targetBackBuffer();

			m_graphicsDevice->getContext()->ClearRenderTargetView(
				m_graphicsDevice->getBackBufferView().Get(),
				clearColor);

			renderBatch.use();

			/*RenderCommand renderCommand = { };
			while (m_commandBuffer->pop(renderCommand))
			{
				renderBatch.setTransformData(renderCommand.modelMatrix);

				m_graphicsDevice->getContext()->Draw(3, 0);
			}*/

			transform.setLocalScale({ 0.25f, 0.25f, 1.0f });
			transform.translate(displacement);
			transform.recalculateModelMatrix();
			renderBatch.setTransformData(transform.getModelMatrix());

			m_graphicsDevice->getContext()->Draw(3, 0);

			m_graphicsDevice->getSwapChain()->Present(1, 0);

			DXGI_FRAME_STATISTICS frameStatistics = { };
			if (m_graphicsDevice->getSwapChain()->GetFrameStatistics(&frameStatistics) == S_OK)
				m_frameTimer.end = frameStatistics.SyncQPCTime.QuadPart;

			double frameDeltaTime = platformGetElapsedSeconds(m_frameTimer);

			displacement.x += 0.125 * frameDeltaTime;

			m_frameTimer.start = m_frameTimer.end;

			m_renderedFrameCount.store(
				m_renderedFrameCount.load(std::memory_order_relaxed) + 1,
				std::memory_order_release);
		}
	}
}