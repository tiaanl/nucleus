
#ifndef NUCLEUS_MACROS_H_
#define NUCLEUS_MACROS_H_

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

// ARRAY_SIZE

// Helper to figure out the item count of a static array of elements, c++ style!
template <typename T, MemSize N>
char (&ArraySizeHelper(T (&array)[N]))[N];
#if COMPILER(MSVC)
template <typename T, MemSize N>
char (&ArraySizeHelper(const T (&array)[N]))[N];
#endif  // COMPILER(MSVC)

#define NU_ARRAY_SIZE(array) (sizeof(ArraySizeHelper(array)))

// IS_BIT_SET

#define NU_BIT_IS_SET(Value, Bit) ((Value & Bit) == Bit)

// UNUSED

#define NU_UNUSED(x)

#endif  // NUCLEUS_MACROS_H_
