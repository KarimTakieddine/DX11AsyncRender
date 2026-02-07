#include <windows.h>

#include "platform.h"

namespace
{
	bool sampleTime(long long& time)
	{
		LARGE_INTEGER counter = { };
		if (QueryPerformanceCounter(&counter) == NULL)
			return false;

		time = counter.QuadPart;

		return true;
	}

	bool getFileAttributes(const char* file, _WIN32_FILE_ATTRIBUTE_DATA* data)
	{
		return GetFileAttributesExA(file, GetFileExInfoStandard, data) != NULL;
	}
}

namespace airful_engine
{
	bool platformStartTime(Timer& timer)
	{
		return sampleTime(timer.start);
	}

	bool platformEndTime(Timer& timer)
	{
		return sampleTime(timer.end);
	}

	double platformGetElapsedSeconds(const Timer& timer)
	{
		LARGE_INTEGER frequency = { };
		if (QueryPerformanceFrequency(&frequency) == NULL)
			return 0.0;

		const LONGLONG quadPart = frequency.QuadPart;

		return quadPart <= 0 ?
			0.0 : double( timer.end - timer.start ) / frequency.QuadPart;
	}

	unsigned long long platformGetFileSize(const char* file)
	{
		_WIN32_FILE_ATTRIBUTE_DATA fileData = { };
		if (!getFileAttributes(file, &fileData))
			return 0;

		return
			( (unsigned long long)fileData.nFileSizeHigh << 32 ) |
			(unsigned long long)fileData.nFileSizeLow;
	}

	bool platformReadFile(const char* file, char* outputBuffer, unsigned long long size)
	{
		HANDLE handle = CreateFileA(
			file,
			GENERIC_READ,
			NULL,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (handle == INVALID_HANDLE_VALUE)
			return false;

		DWORD numberOfBytesRead{ 0 };
		if (ReadFile(handle, outputBuffer, size, &numberOfBytesRead, NULL) == NULL)
			return false;

		CloseHandle(handle);

		return numberOfBytesRead == size;
	}
}