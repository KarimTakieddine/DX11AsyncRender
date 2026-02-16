#pragma once

#include <atomic>
#include <memory>
#include <thread>

#include <Windows.h>

#include "d3d11_shader_store.h"
#include "graphics_device.h"
#include "timer.h"

namespace dx11_async_render
{
	class Win32Window;
	class RenderFrameBuffer;

	class Renderer
	{
	public:
		Renderer() = default;

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		void run(Win32Window* window, RenderFrameBuffer* commandBuffer, const GraphicsConfig& config);
		void stop();
		uint64_t getRenderedFrameCount() const;
		double waitOnFrameCount(uint64_t count);
		double getFrameDeltaTime() const;

	private:
		void initialize(HWND window, const GraphicsConfig& config);
		void execute(HWND window, const GraphicsConfig& config);

		std::thread m_runThread;
		Timer m_frameTimer;
		std::unique_ptr<GraphicsDevice> m_graphicsDevice{ nullptr };
		std::unique_ptr<D3D11ShaderStore> m_shaderStore	{ nullptr };
		Win32Window* m_window							{ nullptr };
		RenderFrameBuffer* m_frameBuffer				{ nullptr };
		std::atomic<uint64_t> m_renderedFrameCount		{ 0 };
		std::atomic<double> m_frameDeltaTime			{ 0.0 };
		std::atomic<bool> m_shouldRun					{ true };
	};
}
