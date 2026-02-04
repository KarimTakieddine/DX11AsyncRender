#include "renderer.h"
#include "win32_window.h"

int main()
{
    airful_engine::Win32Window window("Hello, World!", 640, 480);
    window.show();
    window.update();

    airful_engine::Renderer renderer;

    renderer.run(window.getHandle(), { 640U, 480U, 60U, DXGI_FORMAT_R8G8B8A8_UNORM });

    MSG msg = { 0 };
    while (msg.message != WM_QUIT) {
        // Handle window events (like closing or resizing)
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {

        }
    }

    renderer.stop();

    return 0;
}