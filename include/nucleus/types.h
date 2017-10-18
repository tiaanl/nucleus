
#ifndef NUCLEUS_TYPES_H_
#define NUCLEUS_TYPES_H_

#include <cstddef>
#include <cstdint>

// Common types with fixed sizes.  We put this in the global namespace for convenience.

using I8 = std::int8_t;
using I16 = std::int16_t;
using I32 = std::int32_t;
using I64 = std::int64_t;

using U8 = std::uint8_t;
using U16 = std::uint16_t;
using U32 = std::uint32_t;
using U64 = std::uint64_t;

using F32 = float;
using F64 = double;

using USize = std::size_t;
using PtrDiff = std::ptrdiff_t;

#endif  // NUCLEUS_TYPES_H_
