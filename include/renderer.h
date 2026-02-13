#pragma once

#include <atomic>
#include <memory>
#include <thread>

#include <Windows.h>

#include "d3d11_shader_store.h"
#include "graphics_device.h"
#include "timer.h"

namespace airful_engine
{
	class Win32Window;

	class Renderer
	{
	public:
		Renderer() = default;

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;

		void run(Win32Window* window, const GraphicsConfig& config);
		void stop();

	private:
		void initialize(HWND window, const GraphicsConfig& config);
		void execute(HWND window, const GraphicsConfig& config);

		std::thread m_runThread;
		Timer m_frameTimer;
		std::unique_ptr<GraphicsDevice> m_graphicsDevice{ nullptr };
		std::unique_ptr<D3D11ShaderStore> m_shaderStore	{ nullptr };
		Win32Window* m_window							{ nullptr };
		std::atomic<uint64_t> m_renderedFrameCount		{ 0 };
		std::atomic<bool> m_shouldRun					{ true };
	};
}
