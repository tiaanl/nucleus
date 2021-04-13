#pragma once

#include "nucleus/Macros.h"
#include "nucleus/threading/thread_local.h"

namespace nu {

template <typename T>
class ScopedThreadLocalPtr {
  NU_DELETE_COPY_AND_MOVE(ScopedThreadLocalPtr);

public:
  ScopedThreadLocalPtr() : key_{current_thread::create_storage()} {}

  ScopedThreadLocalPtr(T* ptr) : key_{current_thread::create_storage()} {
    current_thread::set_storage(key_, ptr);
  }

  ~ScopedThreadLocalPtr() {
    const T* value = get();
    delete value;
    current_thread::delete_storage(key_);
  }

  T* get() const {
    return reinterpret_cast<T*>(const_cast<void*>(current_thread::get_storage(key_)));
  }

  T* release() {
    const T* old_value = get();
    current_thread::set_storage(key_, nullptr);
    return old_value;
  }

  void reset(T* ptr) {
    const T* old_value = get();
    delete old_value;
    current_thread::set_storage(key_, ptr);
  }

private:
  current_thread::StorageId key_;
};

}  // namespace nu
