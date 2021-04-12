#include <catch2/catch.hpp>

#include "nucleus/Containers/DynamicArray.h"
#include "nucleus/threading/thread.h"
#include "nucleus/threading/thread_local.h"

namespace nu {

TEST_CASE("ThreadLocal") {
  SECTION("basic") {
    auto id = current_thread::create_storage(nullptr);
    DynamicArray<JoinHandle> handles;
    for (auto i = 0; i < 20; ++i) {
      auto handle = spawn_thread([id]() {
        current_thread::set_storage(id, (void*)&id);
        LOG(Info) << (void*)&id;
        REQUIRE(current_thread::get_storage(id) == (void*)&id);
      });
      handles.emplaceBack(std::move(handle));
    }
    for (auto& handle : handles) {
      handle.join();
    }
  }
}

}  // namespace nu
