#pragma once

#include <DirectXMath.h>

namespace airful_engine
{
	struct D3D11Vertex
	{
		DirectX::XMFLOAT3 position	{ 0.0f, 0.0f, 0.0f };
		DirectX::XMFLOAT4 color		{ 1.0f, 1.0f, 1.0f, 1.0f };
	};
}
