#include <stdexcept>

#include "render_frame_buffer.h"

namespace dx11_async_render
{
    void RenderFrameBuffer::allocate(size_t capacity)
    {
        if ( ( capacity & (capacity - 1) ) != 0)
            throw std::runtime_error("Capacity must be a power of 2!");

        m_data.resize(capacity);
    }

    bool RenderFrameBuffer::push(const RenderFrame& frame) {
        const size_t current_tail   = m_tail.load(std::memory_order_relaxed);
        const size_t next_tail      = (current_tail + 1) & (m_data.size() - 1);

        // Check if buffer is full (head == next_tail)
        if (next_tail == m_head.load(std::memory_order_acquire)) {
            return false;
        }

        m_data[current_tail] = frame;

        // Release: ensure data write finishes before tail update is visible
        m_tail.store(next_tail, std::memory_order_release);

        return true;
    }

    bool RenderFrameBuffer::pop(RenderFrame** frame)
    {
        const size_t current_head = m_head.load(std::memory_order_relaxed);

        // Check if buffer is empty
        if (current_head == m_tail.load(std::memory_order_acquire)) {
            return false;
        }

        *frame = &m_data[current_head];

        // Release: ensure data read finishes before head update is visible
        m_head.store((current_head + 1) & (m_data.size() - 1), std::memory_order_release);

        return true;
    }

    const std::vector<RenderFrame>& RenderFrameBuffer::getData() const
    {
        return m_data;
    }
}