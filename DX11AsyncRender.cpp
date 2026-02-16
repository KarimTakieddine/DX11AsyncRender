#include "bump_allocator.hpp"
#include "d3d11_transform.h"
#include "render_command_buffer.h"
#include "renderer.h"
#include "win32_window.h"

int main()
{
    dx11_async_render::BumpAllocator<8> gameAllocator;
    gameAllocator.allocate(sizeof(dx11_async_render::D3D11Transform));

    auto* transformObject = gameAllocator.requestMemory< dx11_async_render::D3D11Transform>();

    dx11_async_render::Win32Window window("Hello, World!", 1920, 1080);
    window.show();
    window.update();

    dx11_async_render::RenderCommandBuffer renderCommandBuffer;
    renderCommandBuffer.allocate(64);

    dx11_async_render::Renderer renderer;
    renderer.run(&window, &renderCommandBuffer, { 1920U, 1080U, 60U, DXGI_FORMAT_R8G8B8A8_UNORM });

    bool shouldRun{ true };

    DirectX::XMFLOAT3 displacement = { };

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
        // Update frame n
        // Submit commands for frame n

        displacement.x += 0.000016f; // TODO: Replace with real framerate

        transformObject->setLocalScale({ 0.25f, 0.25f, 1.0f });
        transformObject->translate(displacement);
        transformObject->recalculateModelMatrix();

        // while(!renderCommandBuffer.push( { transformObject->getModelMatrix() }));
    }

    renderer.stop();

    return 0;
}