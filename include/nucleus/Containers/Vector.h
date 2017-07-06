
#ifndef NUCLEUS_CONTAINERS_VECTOR_H_
#define NUCLEUS_CONTAINERS_VECTOR_H_

#include "nucleus/Memory/Allocator.h"
#include "nucleus/algorithms/copy.h"
#include "nucleus/types.h"

namespace nu {

template <typename T>
class Vector {
public:
    Vector();
    Vector(Allocator* allocator);
private:
};

}  // namespace nu

#endif  // NUCLEUS_CONTAINERS_VECTOR_H_
