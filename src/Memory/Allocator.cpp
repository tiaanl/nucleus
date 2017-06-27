
#include "nucleus/Memory/Allocator.h"

#include "nucleus/config.h"

#if COMPILER(MSVC)
#include <malloc.h>
#endif

#if COMPILER(GCC) || COMPILER(MINGW)
#include <cstdlib>
#endif

namespace nu {

namespace {

Allocator* gDefaultAllocator = 0;

void* alignedAlloc(USize size, USize alignment) {
#if COMPILER(MSVC)
    return _aligned_malloc(size, alignment);
#elif COMPILER(MINGW)
    return std::malloc(size);
#else
    void* p = nullptr;
    alignment = alignment < sizeof(void*) ? sizeof(void*) : alignment;
    posix_memalign(&p, alignment, size);
    return p;
#endif
}

void alignedFree(void* p) {
#if COMPILER(MSVC)
    _aligned_free(p);
#else
    free(p);
#endif
}

}  // namespace

Allocator::Allocator(const char* name) : name(name) {}

Allocator::Allocator(const Allocator& other) : name(other.name) {}

Allocator::Allocator(const Allocator&, const char* name) : name(name) {}

Allocator& Allocator::operator=(const Allocator& other) {
    name = other.name;

    return *this;
}

void* Allocator::allocate(USize size) {
    return alignedAlloc(size, 16);
}

void Allocator::free(void* ptr, USize size) {
    alignedFree(ptr);
}

const char* Allocator::getName() const {
    return name;
}

void Allocator::setName(const char* aname) {
    name = aname;
}

Allocator* getDefaultAllocator() {
    return gDefaultAllocator;
}

Allocator* setDefaultAllocator(Allocator* allocator) {
    Allocator* previous = gDefaultAllocator;
    gDefaultAllocator = allocator;

    return previous;
}

}  // namespace nu
