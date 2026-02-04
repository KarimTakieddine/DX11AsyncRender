#pragma once

#include "timer.h"

namespace airful_engine
{
	bool platformStartTime(Timer& timer);
	bool platformEndTime(Timer& timer);
	double platformGetElapsedSeconds(const Timer& timer);
}
