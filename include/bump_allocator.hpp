#pragma once

#include <cassert>
#include <cstring>
#include <new>

#define TO_BYTE_PTR(p) (unsigned char*)p

namespace dx11_async_render
{
	template<size_t A>
	class BumpAllocator
	{
		static_assert( !( A & ( A - 1 ) ) ); // Power of 2

	public:
		void allocate(size_t sizeInBytes)
		{
			release();

			const auto unalignedStart = (uintptr_t)malloc(sizeInBytes + A);

			const size_t mask				= A - 1;
			const uintptr_t misalignment	= ( unalignedStart & mask );
			const ptrdiff_t adjustment		= A - misalignment;
			const uintptr_t alignedStart	= unalignedStart + adjustment;

			assert(adjustment <= 0xFF);

			auto * const pAlignedStart	= TO_BYTE_PTR(alignedStart);
			pAlignedStart[-1]			= static_cast<unsigned char>(adjustment);

			m_buffer	= pAlignedStart;
			m_position	= m_buffer;
			m_bounds	= TO_BYTE_PTR(m_buffer) + sizeInBytes;

			memset(m_buffer, NULL, sizeInBytes);
		}

		template<typename T>
		T* requestMemory()
		{
			static_assert(sizeof(T) % A == 0);

			const size_t sizeInBytes = sizeof(T);

			auto* const nextPosition = TO_BYTE_PTR(m_position) + sizeInBytes;

			if (nextPosition > m_bounds)
				return nullptr;

			T* result = new (m_position) T();

			m_position = nextPosition;

			return result;
		}

		template<typename T>
		T* requestMemory(const T& value = T())
		{
			static_assert(sizeof(T) % A == 0);

			const size_t sizeInBytes = sizeof(T);

			auto * const nextPosition = TO_BYTE_PTR(m_position) + sizeInBytes;

			if (nextPosition > m_bounds)
				return nullptr;

			T* result = new (m_position) T(value);

			m_position = nextPosition;

			return result;
		}

		void freeMemory(size_t sizeInBytes)
		{
			auto* nextPosition = TO_BYTE_PTR(m_position) - sizeInBytes;

			if (nextPosition < m_buffer)
			{
				nextPosition = TO_BYTE_PTR(m_buffer);
			}

			m_position = nextPosition;
		}

		void release()
		{
			if (!m_buffer)
				return;

			const auto* pAlignedStart	= TO_BYTE_PTR(m_buffer);
			const ptrdiff_t adjustment	= static_cast<ptrdiff_t>(pAlignedStart[-1]);

			const uintptr_t alignedStart	= (uintptr_t)m_buffer;
			const uintptr_t unalignedStart	= alignedStart - adjustment;

			free((void*)unalignedStart);

			m_buffer	= nullptr;
			m_bounds	= nullptr;
			m_position	= nullptr;
		}

		ptrdiff_t getSizeInBytes()
		{
			return TO_BYTE_PTR(m_bounds) - TO_BYTE_PTR(m_buffer);
		}

		ptrdiff_t getLiveSizeInBytes()
		{
			return TO_BYTE_PTR(m_position) - TO_BYTE_PTR(m_buffer);
		}

		ptrdiff_t getInactiveSizeInBytes()
		{
			return TO_BYTE_PTR(m_bounds) - TO_BYTE_PTR(m_position);
		}

		void* peek() const
		{
			return m_buffer;
		}

		~BumpAllocator()
		{
			release();
		}

	private:
		void* m_bounds		{ nullptr };
		void* m_position	{ nullptr };
		void* m_buffer		{ nullptr };
	};
}
