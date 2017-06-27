
#ifndef NUCLEUS_MEMORY_ALLOCATOR_H_
#define NUCLEUS_MEMORY_ALLOCATOR_H_

#include "nucleus/types.h"

namespace nu {

class Allocator {
public:
    explicit Allocator(const char* name = "Allocator");
    Allocator(const Allocator& other);
    Allocator(const Allocator& other, const char* name);

    Allocator& operator=(const Allocator& other);

    void* allocate(USize size);
    void free(void* ptr, USize size);

    const char* getName() const;
    void setName(const char* name);

protected:
    const char* name;
};

bool operator==(const Allocator& left, const Allocator& right);
bool operator!=(const Allocator& left, const Allocator& right);

Allocator* getDefaultAllocator();
Allocator* setDefaultAllocator(Allocator* allocator);

}  // namespace nu

#endif  // NUCLEUS_MEMORY_ALLOCATOR_H_
