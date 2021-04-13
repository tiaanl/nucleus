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
#elif OS(POSIX)
using StorageId = pthread_key_t;
#endif

StorageId create_storage();
void delete_storage(StorageId id);
void set_storage(StorageId id, const void* value);
const void* get_storage(StorageId id);

}  // namespace nu::current_thread
