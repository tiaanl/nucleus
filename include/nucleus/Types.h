
#ifndef NUCLEUS_TYPES_H_
#define NUCLEUS_TYPES_H_

#include "nucleus/Config.h"

using I8 = char;
using I16 = short;
using I32 = int;
using I64 = long long;

static_assert(sizeof(I8) == 1, "Invalid size for I8");
static_assert(sizeof(I16) == 2, "Invalid size for I16");
static_assert(sizeof(I32) == 4, "Invalid size for I32");
static_assert(sizeof(I64) == 8, "Invalid size for I64");

using U8 = unsigned char;
using U16 = unsigned short;
using U32 = unsigned int;
using U64 = unsigned long long;

static_assert(sizeof(U8) == 1, "Invalid size for U8");
static_assert(sizeof(U16) == 2, "Invalid size for U16");
static_assert(sizeof(U32) == 4, "Invalid size for U32");
static_assert(sizeof(U64) == 8, "Invalid size for U64");

using F32 = float;
using F64 = double;

static_assert(sizeof(F32) == 4, "Invalid size for F32");
static_assert(sizeof(F64) == 8, "Invalid size for F64");

#if ARCH(CPU_64_BITS)
using USize = U64;
using PtrDiff = I64;
#elif ARCH(CPU_32_BITS)
using USize = U32;
using PtrDiff = I32;
#else
#error "What kind of crazy CPU do you have?!"
#endif

// Type used for max alignment size.
using MaxAlign = long double;

#endif  // NUCLEUS_TYPES_H_
