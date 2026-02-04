#include <windows.h>

#include "graphics_device.h"
#include "win32_window.h"

int main()
{
    // ShowWindow(windowHandle, SW_SHOWDEFAULT);
    // UpdateWindow(windowHandle);

    airful_engine::Win32Window window("Hello, World!", 640, 480);

    airful_engine::GraphicsDevice graphicsDevice(
        window.getHandle(),
        640,
        480,
        60,
        DXGI_FORMAT_R8G8B8A8_UNORM
    );

    //MSG msg = { 0 };
    //while (msg.message != WM_QUIT) {
    //    // Handle window events (like closing or resizing)
    //    if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
    //        TranslateMessage(&msg);
    //        DispatchMessage(&msg);
    //        continue; // Continue loop to check WM_QUIT
    //    }
    //}

    // UnregisterClassA("hello_dx11_world_window", instance);

    return 0;
}