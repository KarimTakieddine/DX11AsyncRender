#pragma once

#include "timer.h"

namespace airful_engine
{
	extern "C" bool platformStartTime(Timer& timer);
	extern "C" bool platformEndTime(Timer& timer);
	extern "C" double platformGetElapsedSeconds(const Timer& timer);
	extern "C" unsigned long long platformGetFileSize(const char* file);
	extern "C" bool platformReadFile(const char* file, char* outputBuffer, unsigned long long size);
}
