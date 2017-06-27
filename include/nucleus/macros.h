// Copyright (c) 2015, Tiaan Louw
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
// REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
// INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
// LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
// OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.

#ifndef NUCLEUS_MACROS_H_
#define NUCLEUS_MACROS_H_

#include "nucleus/config.h"
#include "nucleus/types.h"

#define DISALLOW_COPY_AND_ASSIGN(ClassName)                                    \
    ClassName(const ClassName&) = delete;                                      \
    ClassName& operator=(const ClassName&) = delete

#define DISALLOW_IMPLICIT_CONSTRUCTORS(ClassName)                              \
    DISALLOW_COPY_AND_ASSIGN(ClassName);                                       \
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
