#include "renderer.h"
#include "win32_window.h"

int main()
{
    airful_engine::Win32Window window("Hello, World!", 640, 480);
    window.show();
    window.update();

    airful_engine::Renderer renderer;
    renderer.run(window.getHandle(), { 640U, 480U, 60U, DXGI_FORMAT_R8G8B8A8_UNORM });

    bool shouldRun{ true };

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
    }

    renderer.stop();

    return 0;
}