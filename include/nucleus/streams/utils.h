#pragma once

#include "nucleus/Containers/dynamic_array.h"
#include "nucleus/Streams/input_stream.h"

namespace nu {

DynamicArray<U8> readEntireStream(InputStream* inputStream);

}  // namespace nu
