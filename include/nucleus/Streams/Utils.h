#ifndef NUCLEUS_STREAMS_UTILS_H_
#define NUCLEUS_STREAMS_UTILS_H_

#include "nucleus/Containers/DynamicArray.h"
#include "nucleus/Streams/InputStream.h"

namespace nu {

DynamicArray<U8> readEntireStream(InputStream* inputStream);

}  // namespace nu

#endif  // NUCLEUS_STREAMS_UTILS_H_
