#include <catch2/catch.hpp>

#include "nucleus/Containers/DynamicArray.h"
#include "nucleus/task.h"
#include "nucleus/threading/thread.h"
#include "nucleus/threading/thread_local.h"

namespace nu {

TEST_CASE("ThreadLocal") {
  SECTION("basic") {
    class CheckThreadId : public Task {
    public:
      explicit CheckThreadId(current_thread::StorageId storage_id) : storage_id_{storage_id} {}
      ~CheckThreadId() override = default;

      void execute() override {
        current_thread::set_storage(storage_id_, this);
        LOG(Info) << this;
        REQUIRE(current_thread::get_storage(storage_id_) == this);
      }

    private:
      current_thread::StorageId storage_id_;
    };

    auto id = current_thread::create_storage(nullptr);
    DynamicArray<JoinHandle> handles;
    for (auto i = 0; i < 20; ++i) {
      auto handle = spawn_thread(makeScopedPtr<CheckThreadId>(id));
      handles.emplaceBack(std::move(handle));
    }
    for (auto& handle : handles) {
      handle.join();
    }
  }
}

}  // namespace nu
