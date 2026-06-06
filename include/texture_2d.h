#pragma once

enum class TextureUsage
{
	DEFAULT,
	IMMUTABLE,
	DYNAMIC,
	STAGING
};

enum class WrapMode
{
	WRAP,
	MIRROR,
	CLAMP,
	BORDER
};

struct Texture2D
{
	unsigned int width			{ 0 };
	unsigned int height			{ 0 };
	unsigned int maxMipMapLevels{ 0 };
	unsigned int sampleCount	{ 0 };
	unsigned int sampleQuality	{ 0 };
};
