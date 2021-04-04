#pragma once

#include "nucleus/Config.h"
#include "nucleus/Macros.h"

#if OS(POSIX)
#include <pthread.h>
#endif  // OS(POSIX)

namespace nu {

namespace detail {

#if OS(POSIX)
using ThreadLocalStorageKeyType = pthread_key_t;

inline ThreadLocalStorageKeyType create_thread_local() {
  ThreadLocalStorageKeyType key;
  // TODO: Check for error.
  pthread_key_create(&key, nullptr);
  return key;
}

inline ThreadLocalStorageKeyType create_thread_local(const void* value) {
  ThreadLocalStorageKeyType key;
  // TODO: Check for error.
  pthread_key_create(&key, nullptr);
  pthread_setspecific(key, value);
  return key;
}

inline void delete_thread_local(ThreadLocalStorageKeyType key) {
  // TODO: Check for error.
  pthread_key_delete(key);
}

inline void set_thread_local(ThreadLocalStorageKeyType key, const void* value) {
  // TODO: Check for error.
  pthread_setspecific(key, value);
}

inline const void* get_thread_local(ThreadLocalStorageKeyType key) {
  // TODO: Check for error.
  return pthread_getspecific(key);
}

#endif  // OS(POSIX)

}  // namespace detail

template <typename T>
class ScopedThreadLocalPtr {
  NU_DELETE_COPY_AND_MOVE(ScopedThreadLocalPtr);

public:
  ScopedThreadLocalPtr() : key_{detail::create_thread_local()} {}

  ScopedThreadLocalPtr(T* ptr) : key_{detail::create_thread_local(ptr)} {}

  ~ScopedThreadLocalPtr() {
    const T* value = get();
    delete value;
    detail::delete_thread_local(key_);
  }

  T* get() const {
    return reinterpret_cast<T*>(const_cast<void*>(detail::get_thread_local(key_)));
  }

  void reset(T* ptr) {
    const T* old_value = get();
    delete old_value;
    detail::set_thread_local(key_, ptr);
  }

private:
  detail::ThreadLocalStorageKeyType key_;
};

}  // namespace nu
