#pragma once

#include <windows.h>

namespace airful_engine
{
	class Win32Window
	{
	public:
		explicit Win32Window(const char* title, int width, int height);

		Win32Window(const Win32Window&) = delete;
		Win32Window& operator=(const Win32Window&) = delete;

		~Win32Window();

		HWND getHandle() const;

	private:
		HWND m_handle{ NULL };
	};
}