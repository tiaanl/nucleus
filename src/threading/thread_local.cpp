#include "nucleus/threading/thread_local.h"

namespace nu::current_thread {

#if OS(WIN)
StorageId create_storage() {
  return TlsAlloc();
}

StorageId create_storage(void* value) {
  StorageId id = TlsAlloc();
  TlsSetValue(id, value);
  return id;
}

void delete_storage(StorageId id) {
  TlsFree(id);
}

void set_storage(StorageId id, const void* value) {
  TlsSetValue(id, const_cast<void*>(value));
}

const void* get_storage(StorageId id) {
  return TlsGetValue(id);
}

#elif OS(POSIX)

StorageId create_storage() {
  StorageId id;
  // TODO: Check for error.
  pthread_key_create(&id, nullptr);
  return id;
}

void delete_storage(StorageId id) {
  // TODO: Check for error.
  pthread_key_delete(id);
}

void set_storage(StorageId id, const void* value) {
  // TODO: Check for error.
  pthread_setspecific(id, value);
}

const void* get_storage(StorageId id) {
  // TODO: Check for error.
  return pthread_getspecific(id);
}

#endif  // OS(POSIX)

}  // namespace nu::current_thread
