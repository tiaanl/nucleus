
#ifndef NUCLEUS_ALLOCATORS_LINEAR_ALLOCATOR_H
#define NUCLEUS_ALLOCATORS_LINEAR_ALLOCATOR_H

#include "nucleus/Allocators/Allocator.h"
#include "nucleus/types.h"

namespace nu {

    class LinearAllocator : public Allocator {
    public:
        LinearAllocator(Allocator* parentAllocator, USize totalSize);
        ~LinearAllocator() override;

    protected:
        void *doAllocate(USize bytes, USize alignment) override;
        void doFree(void *p, USize bytes, USize alignment) override;
        bool doIsEqual(const Allocator &other) const noexcept override;

    private:
        Allocator* m_parentAllocator;
        USize m_totalSize;
        void* m_start;
        USize m_offset;
    };

}  // namespace nu

#endif  // NUCLEUS_ALLOCATORS_LINEAR_ALLOCATOR_H
