#pragma once

#include "nucleus/logging.h"

namespace nu {

// StablePool stores T's with the following guarantees:
// - once an object is inserted, it's pointer will never change.
// - items will be tightly packed.
template <typename T, MemSize PoolSize = 16>
class StablePool {
public:
  StablePool() = default;

  ~StablePool() {
    Pool* pool = first_pool_;
    while (pool) {
      pool->clear();
      Pool* current = pool;
      pool = pool->next;
      std::free(current);
    }
  }

  MemSize size() const {
    return size_;
  }

  MemSize capacity() const {
    return capacity_;
  }

  template <typename... Args>
  T* construct(Args&&... args) {
    T* slot = get_slot_for_writing();

    new (slot) T(std::forward<Args>(args)...);

    size_ += 1;

    return slot;
  }

  T* insert(T&& item) {
    T* slot = get_slot_for_writing();

    new (slot) T(std::forward<T>(item));

    size_ += 1;

    return slot;
  }

  bool remove(T* ptr) {
    for (auto* pool = first_pool_; pool; pool = pool->next) {
      if (pool->contains(ptr)) {
        pool->remove(ptr);
        return true;
      }
    }

    return false;
  }

private:
  static_assert(PoolSize >= 8 && PoolSize <= 64 && PoolSize % 8 == 0,
                "Allowed sizes are 8, 16, 24, 32, 40, 48, 56, 64");
  struct Pool {
    U8 data[sizeof(T) * PoolSize];
    // TODO: This can be a smaller type.
    MemSize occupied;
    Pool* next;

    bool is_occupied(MemSize index) const {
      DCHECK(index < PoolSize);
      return (occupied & (static_cast<MemSize>(1) << index)) != 0;
    }

    void occupy(MemSize index) {
      DCHECK(index < PoolSize);
      occupied |= (static_cast<MemSize>(1) << index);
    }

    void vacate(MemSize index) {
      DCHECK(index < PoolSize);
      occupied &= ~(static_cast<MemSize>(1) << index);
    }

    bool has_open_slots() const {
      return occupied != static_cast<MemSize>(1) << PoolSize;
    }

    T* at(MemSize index) {
      DCHECK(index < PoolSize);
      return reinterpret_cast<T*>(&data[index * sizeof(T)]);
    }

    T* get_slot_for_writing() {
      for (MemSize i = 0; i < PoolSize; ++i) {
        if (!is_occupied(i)) {
          occupy(i);
          return at(i);
        }
      }

      return nullptr;
    }

    void clear() {
      for (MemSize i = 0; i < PoolSize; ++i) {
        if (is_occupied(i)) {
          at(i)->~T();
        }
      }
    }

    bool contains(T* ptr) const {
      auto ptr_addr = reinterpret_cast<MemSize>(ptr);
      auto data_addr = reinterpret_cast<MemSize>(data);
      return ptr_addr >= data_addr && ptr_addr < data_addr + PoolSize * sizeof(T);
    }

    void remove(T* ptr) {
      DCHECK(contains(ptr));
      PtrDiff index = reinterpret_cast<MemSize>(ptr) - reinterpret_cast<MemSize>(data);
      DCHECK(index % sizeof(T) == 0);
      index /= sizeof(T);
      vacate(index);
      at(index)->~T();
    }
  };

  Pool* allocate_pool() {
    auto* new_pool = static_cast<Pool*>(std::malloc(sizeof(Pool)));
    new_pool->occupied = 0;
    new_pool->next = nullptr;
    return new_pool;
  }

  T* get_slot_for_writing() {
    T* slot = nullptr;

    if (size_ < capacity_) {
      for (Pool* pool = first_pool_; pool; pool = pool->next) {
        if (pool->has_open_slots()) {
          slot = pool->get_slot_for_writing();
        }
      }
    } else {
      Pool* new_pool = allocate_pool();
      new_pool->next = first_pool_;
      first_pool_ = new_pool;
      capacity_ += PoolSize;
      slot = first_pool_->get_slot_for_writing();
    }

    DCHECK(slot != nullptr);

    return slot;
  }

  MemSize size_ = 0;
  MemSize capacity_ = 0;
  Pool* first_pool_ = nullptr;
};

}  // namespace nu
