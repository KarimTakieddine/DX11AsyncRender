#pragma once

#include <dxgiformat.h>

namespace airful_engine
{
	struct GraphicsConfig
	{
		unsigned int width	{ 0 };
		unsigned int height	{ 0 };
		unsigned int fps	{ 0 };
		DXGI_FORMAT format	{ DXGI_FORMAT_UNKNOWN };
	};
}
