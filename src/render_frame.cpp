#include "render_frame.h"

namespace dx11_async_render
{
	void RenderFrame::addRenderCommand(const RenderCommand& command)
	{
		m_renderCommands.push_back(command);
	}

	const std::vector<RenderCommand>& RenderFrame::getRenderCommands() const
	{
		return m_renderCommands;
	}
}