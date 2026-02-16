#pragma once

#include <atomic>

#include "render_frame.h"

namespace dx11_async_render
{
    class RenderFrameBuffer
    {
    public:
        void allocate(size_t capacity);
        bool push(const RenderFrame& frame);
        bool pop(RenderFrame** frame);

        const std::vector<RenderFrame>& getData() const;

    private:
        std::vector<RenderFrame> m_data;

        alignas(64) std::atomic<size_t> m_head{ 0 };
        alignas(64) std::atomic<size_t> m_tail{ 0 };
    };
}
