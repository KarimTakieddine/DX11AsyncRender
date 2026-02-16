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

    dx11_async_render::Win32Window* getWindow(LPARAM lParam)
    {
       return (dx11_async_render::Win32Window*)( (CREATESTRUCT*)lParam )->lpCreateParams;
    }

    LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg) {
        case WM_NCCREATE:
        {
            SetWindowLongPtrA(hWnd, 0, (LONG_PTR)getWindow(lParam));
            break;
        }
        case WM_SIZE:
        {
            auto* window = (dx11_async_render::Win32Window*)GetWindowLongPtrA(hWnd, 0);
            window->handleResize(LOWORD(lParam), HIWORD(lParam));
            return 0;
        }
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

namespace dx11_async_render
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
            windowConfig.cbWndExtra     = sizeof(LONG_PTR);
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
            this);

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

    BOOL Win32Window::show()
    {
        return ShowWindow(m_handle, SW_SHOW);
    }

    BOOL Win32Window::update()
    {
        return UpdateWindow(m_handle);
    }

    void Win32Window::handleResize(UINT width, UINT height)
    {
        enqueueMessage(
            {
                MessageType::RESIZE,
                ( (static_cast<uint64_t>(width) & 0xFFFFFFFF) << 32 ) | height
            });
    }

    BOOL Win32Window::dequeMessage(Message& message)
    {
        // TODO(Karim): Replace with 64 bit atomic - type (32) + width(16) + height(16)

        std::lock_guard<std::mutex> lock(m_messageQueueMutex);

        if (m_messageQueue.empty())
            return FALSE;

        message = m_messageQueue.front();

        m_messageQueue.pop_front();

        return TRUE;
    }

    void Win32Window::enqueueMessage(const Message& message)
    {
        std::lock_guard<std::mutex> lock(m_messageQueueMutex);

        m_messageQueue.push_back(message);
    }
}