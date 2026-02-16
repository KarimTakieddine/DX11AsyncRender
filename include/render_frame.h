#pragma once

#include <vector>

#include "render_command.h"

namespace dx11_async_render
{
	class RenderFrame
	{
	public:
		void addRenderCommand(const RenderCommand& command);
		const std::vector<RenderCommand>& getRenderCommands() const;

	private:
		std::vector<RenderCommand> m_renderCommands;
	};
}
