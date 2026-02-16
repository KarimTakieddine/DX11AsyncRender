#include "bump_allocator.hpp"
#include "d3d11_transform.h"
#include "render_frame_buffer.h"
#include "renderer.h"
#include "win32_window.h"

#include <iostream>

int main()
{
    const uint64_t maxQueuedFrames = 4;

    dx11_async_render::BumpAllocator<8> gameAllocator;
    gameAllocator.allocate(sizeof(dx11_async_render::D3D11Transform));

    auto* transformObject = gameAllocator.requestMemory< dx11_async_render::D3D11Transform>();

    dx11_async_render::Win32Window window("Hello, World!", 1920, 1080);
    window.show();
    window.update();

    dx11_async_render::RenderFrameBuffer renderFrameBuffer;
    renderFrameBuffer.allocate(64);

    dx11_async_render::Renderer renderer;
    renderer.run(&window, &renderFrameBuffer, { 1920U, 1080U, 60U, DXGI_FORMAT_R8G8B8A8_UNORM });

    bool shouldRun{ true };

    DirectX::XMFLOAT3 displacement = { };

    uint64_t command_index = 1;

    while (shouldRun)
    {
        MSG msg = { 0 };
        while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
            {
                shouldRun = false;
                break;
            }
        }

        if (!shouldRun)
            break;

        // Update input
        // Wait for frame n - k to be rendered
        // Sample delta time

        double frameDeltaTime = 0.0;

        if (command_index > maxQueuedFrames)
        {
            const uint64_t frameToWaitFor = command_index - maxQueuedFrames;
            
            frameDeltaTime = renderer.waitOnFrameCount(frameToWaitFor);
        }

        // Update frame n:

        dx11_async_render::RenderFrame renderFrame;

        displacement.x += 0.25f * frameDeltaTime;
        transformObject->setLocalScale({ 0.25f, 0.25f, 1.0f });
        transformObject->translate(displacement);
        transformObject->recalculateModelMatrix();

        renderFrame.addRenderCommand( { transformObject->getModelMatrix() });

        // Submit commands for frame n

        while (!renderFrameBuffer.push(renderFrame));

        ++command_index;
    }

    renderer.stop();

    return 0;
}