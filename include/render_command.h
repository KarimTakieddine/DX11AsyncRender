#pragma once

#include <DirectXMath.h>

namespace dx11_async_render
{
	struct RenderCommand
	{
		DirectX::XMMATRIX modelMatrix{ DirectX::XMMatrixIdentity() };
	};
}
