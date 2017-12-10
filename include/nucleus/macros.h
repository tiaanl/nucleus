
#ifndef NUCLEUS_MACROS_H_
#define NUCLEUS_MACROS_H_

#include "nucleus/config.h"
#include "nucleus/types.h"

#define DISALLOW_COPY_AND_ASSIGN(ClassName)                                                                            \
public:                                                                                                                \
  ClassName(const ClassName&) = delete;                                                                                \
  ClassName& operator=(const ClassName&) = delete

#define DISALLOW_IMPLICIT_CONSTRUCTORS(ClassName)                                                                      \
  DISALLOW_COPY_AND_ASSIGN(ClassName);                                                                                 \
  ClassName() = delete

// ARRAY_SIZE

// Helper to figure out the item count of a static array of elements, c++ style!
template <typename T, USize N>
char (&ArraySizeHelper(T (&array)[N]))[N];
#if COMPILER(MSVC)
template <typename T, USize N>
char (&ArraySizeHelper(const T (&array)[N]))[N];
#endif  // COMPILER(MSVC)

#define ARRAY_SIZE(array) (sizeof(ArraySizeHelper(array)))

// IS_BIT_SET

#define IS_BIT_SET(Value, Bit) ((Value & Bit) == Bit)

#endif  // NUCLEUS_MACROS_H_
