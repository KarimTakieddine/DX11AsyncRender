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
			0.0 : static_cast<double>( timer.end - timer.start ) / frequency.QuadPart;
	}
}