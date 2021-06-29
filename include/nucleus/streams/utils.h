#pragma once

#include "nucleus/containers/dynamic_array.h"
#include "nucleus/streams/input_stream.h"

namespace nu {

DynamicArray<U8> readEntireStream(InputStream* inputStream);

}  // namespace nu
