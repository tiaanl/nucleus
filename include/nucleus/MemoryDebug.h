
#ifndef NUCLEUS_MEMORY_DEBUG_H_
#define NUCLEUS_MEMORY_DEBUG_H_

#include <new>

#if 0
void* operator new(std::size_t size);

void operator delete(void* ptr) noexcept;
#endif  // 0

#ifdef _DEBUG
#define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

#endif  // NUCLEUS_MEMORY_DEBUG_H_
