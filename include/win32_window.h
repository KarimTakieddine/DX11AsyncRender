#pragma once

#include <deque>
#include <mutex>

#include <Windows.h>

namespace airful_engine
{
	class Win32Window
	{
	public:
		enum class MessageType : uint64_t
		{
			UNKNOWN = 0,
			RESIZE	= 1
		};

		struct Message
		{
			MessageType type{ MessageType::UNKNOWN };
			uint64_t data	{ 0 };
		};

		explicit Win32Window(const char* title, int width, int height);

		Win32Window(const Win32Window&) = delete;
		Win32Window& operator=(const Win32Window&) = delete;

		~Win32Window();

		HWND getHandle() const;

		BOOL show();

		BOOL update();

		BOOL handleResize(UINT width, UINT height);

	private:
		void enqueueMessage(const Message& message);

		std::mutex m_messageQueueMutex;
		std::deque<Message> m_messageQueue;
		HWND m_handle{ NULL };
	};
}