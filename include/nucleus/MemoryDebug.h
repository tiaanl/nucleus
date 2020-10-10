#ifndef NUCLEUS_MEMORY_DEBUG_H_
#define NUCLEUS_MEMORY_DEBUG_H_

#if 0
#include <new>

#if BUILD(DEBUG) && COMPILER(MSVC)
#define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif
#endif  // 0

#endif  // NUCLEUS_MEMORY_DEBUG_H_
