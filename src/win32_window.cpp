#pragma once

#include <stdexcept>

#include "win32_window.h"

namespace
{
	static constexpr const char* WINDOW_CLASS_NAME = "airful_engine_window";

    /*
    * If this parameter is NULL, GetModuleHandle returns a handle
    * to the file used to create the calling process (.exe file).
    */
    static HINSTANCE applicationInstance = GetModuleHandle(NULL);

    LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg) {
        case WM_SIZE:
            // Handle resizing DX11 buffers here
            return 0;
        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE) PostQuitMessage(0);
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        }
        // Let Windows handle everything we don't care about (like drawing the title bar)
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}

namespace airful_engine
{
	Win32Window::Win32Window(const char* title, int width, int height)
	{
        WNDCLASSEXA windowConfig = { };
        if (!GetClassInfoExA(applicationInstance, WINDOW_CLASS_NAME, &windowConfig))
        {
            /*
            * Registers a window class for subsequent use in calls to the
            * CreateWindow or CreateWindowEx functions.
            */

            windowConfig.cbSize         = sizeof(WNDCLASSEX);
            windowConfig.style          = CS_HREDRAW | CS_VREDRAW;
            windowConfig.lpfnWndProc    = &::WndProc;
            windowConfig.cbClsExtra     = 0;
            windowConfig.cbWndExtra     = 0;
            windowConfig.hInstance      = applicationInstance;
            windowConfig.hIconSm        = NULL;
            windowConfig.hIcon          = NULL;
            windowConfig.hCursor        = NULL;
            windowConfig.hbrBackground  = GetSysColorBrush(COLOR_BACKGROUND);
            windowConfig.lpszMenuName   = NULL;
            windowConfig.lpszClassName  = WINDOW_CLASS_NAME;

            RegisterClassExA(&windowConfig);
        }

        HWND windowHandle = CreateWindowExA(
            0,
            WINDOW_CLASS_NAME,
            title,
            // WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX
            WS_OVERLAPPEDWINDOW,
            0, 0,
            width, height,
            NULL,
            NULL,
            applicationInstance,
            nullptr);

        if (windowHandle == NULL)
        {
            // TODO(Karim): General function for mapping GetLastError() to string
            throw std::runtime_error("Failed to create window");
        }

        m_handle = windowHandle;
	}

	Win32Window::~Win32Window()
	{
        UnregisterClassA("airful_engine_window", applicationInstance);

		DestroyWindow(m_handle);
	}

    HWND Win32Window::getHandle() const
    {
        return m_handle;
    }
}