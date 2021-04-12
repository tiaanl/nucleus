#pragma once

#include "nucleus/Config.h"

#if OS(WIN)
#include "nucleus/Win/WindowsMixin.h"
#elif OS(POSIX)
#include <pthread.h>
#endif  // OS(POSIX

namespace nu::current_thread {

#if OS(WIN)
using StorageId = DWORD;

inline StorageId create_storage() {
  return TlsAlloc();
}

inline StorageId create_storage(void* value) {
  StorageId id = TlsAlloc();
  TlsSetValue(id, value);
  return id;
}

inline void delete_storage(StorageId id) {
  TlsFree(id);
}

inline void set_storage(StorageId id, void* value) {
  TlsSetValue(id, const_cast<void*>(value));
}

inline void* get_storage(StorageId id) {
  return TlsGetValue(id);
}

#elif OS(POSIX)
using StorageId = pthread_key_t;

inline StorageId create_storage() {
  StorageId id;
  // TODO: Check for error.
  pthread_key_create(&id, nullptr);
  return id;
}

inline StorageId create_storage(void* value) {
  StorageId id;
  // TODO: Check for error.
  pthread_key_create(&id, nullptr);
  pthread_setspecific(id, value);
  return id;
}

inline void delete_storage(StorageId id) {
  // TODO: Check for error.
  pthread_key_delete(id);
}

inline void set_storage(StorageId id, void* value) {
  // TODO: Check for error.
  pthread_setspecific(id, value);
}

inline void* get_storage(StorageId id) {
  // TODO: Check for error.
  return pthread_getspecific(id);
}

#endif  // OS(POSIX)

}  // namespace nu::current_thread
