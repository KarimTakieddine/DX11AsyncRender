#include <thread>

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

		platformStartTime(m_frameTimer);
	}

	void Renderer::execute(HWND window, const GraphicsConfig& config)
	{
		initialize(window, config);

		while (m_shouldRun.load(std::memory_order_acquire))
		{
			float clearColor[] = { 1.0f, 0.71f, 0.76f, 1.0f };

			m_graphicsDevice->getContext()->ClearRenderTargetView(
				m_graphicsDevice->getBackBufferView().Get(),
				clearColor);

			m_graphicsDevice->getSwapChain()->Present(1, 0);

			DXGI_FRAME_STATISTICS frameStatistics = { };
			if (m_graphicsDevice->getSwapChain()->GetFrameStatistics(&frameStatistics) == S_OK)
				m_frameTimer.end = frameStatistics.SyncQPCTime.QuadPart;

			

			m_frameTimer.start = m_frameTimer.end;
		}
	}
}