#pragma once

#include "nucleus/Config.h"
#include "nucleus/Types.h"

#define NU_DELETE_COPY(ClassName)                                                                  \
  ClassName(const ClassName&) = delete;                                                            \
  ClassName& operator=(const ClassName&) = delete

#define NU_DELETE_MOVE(ClassName)                                                                  \
  ClassName(ClassName&&) = delete;                                                                 \
  ClassName& operator=(ClassName&&) = delete

#define NU_DELETE_COPY_AND_MOVE(ClassName)                                                         \
public:                                                                                            \
  NU_DELETE_COPY(ClassName);                                                                       \
  NU_DELETE_MOVE(ClassName)

#define NU_DEFAULT_COPY(ClassName)                                                                 \
  ClassName(const ClassName&) = default;                                                           \
  ClassName& operator=(const ClassName&) = default

#define NU_DEFAULT_MOVE(ClassName)                                                                 \
  ClassName(ClassName&&) = default;                                                                \
  ClassName& operator=(ClassName&&) = default

// INLINE

#if COMPILER(GCC)
#define NU_NEVER_INLINE [[gnu::noinline]]
#elif COMPILER(MSVC)
#define NU_NEVER_INLINE __declspec(noinline)
#else
#error Unknown compiler.
#endif

// ARRAY_SIZE

// Helper to figure out the item count of a static array of elements, c++ style!
template <typename T, MemSize N>
char (&array_size_helper(T (&array)[N]))[N];
#if COMPILER(MSVC)
template <typename T, MemSize N>
char (&array_size_helper(const T (&array)[N]))[N];
#endif  // COMPILER(MSVC)

#define NU_ARRAY_SIZE(array) (sizeof(array_size_helper(array)))

// IS_BIT_SET

#define NU_BIT(Bit) ((MemSize(1) << MemSize(Bit)))
#define NU_BIT_IS_SET(Value, Bit) ((MemSize(Value) & MemSize(Bit)) == MemSize(Bit))

// UNUSED

#define NU_UNUSED(x)

#define NU_NO_DISCARD [[nodiscard]]
