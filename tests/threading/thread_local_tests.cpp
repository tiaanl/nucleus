#include <catch2/catch.hpp>

#include "nucleus/containers/dynamic_array.h"
#include "nucleus/threading/thread.h"
#include "nucleus/threading/thread_local.h"

namespace nu {

TEST_CASE("ThreadLocal") {
  SECTION("basic") {
    auto id = current_thread::create_storage();

    nu::DynamicArray<JoinHandle> handles;
    for (int i = 0; i < 30; ++i) {
      handles.emplaceBack(spawn_thread([id, i]() {
        auto value = (const void*)(MemSize)i;
        current_thread::set_storage(id, value);
        REQUIRE(current_thread::get_storage(id) == value);
      }));
    }

    for (auto& handle : handles) {
      handle.join();
    }
  }
}

}  // namespace nu
