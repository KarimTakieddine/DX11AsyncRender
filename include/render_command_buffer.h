#include <atomic>
#include <vector>

#include "render_command.h"

namespace dx11_async_render
{
    class RenderCommandBuffer
    {
    public:
        void allocate(size_t capacity);
        bool push(const RenderCommand& command);
        bool pop(RenderCommand& command);

    private:
        std::vector<RenderCommand> m_data;

        alignas(64) std::atomic<size_t> m_head{ 0 };
        alignas(64) std::atomic<size_t> m_tail{ 0 };
    };
}
