#include <catch2/catch.hpp>

#include "nucleus/containers/hash_table.h"
#include "nucleus/testing/lifetime_tracker.h"

namespace nu {

using testing::LifetimeTracker;

TEST_CASE("HashTable") {
  LifetimeTracker::reset();

  SECTION("default construct") {
    HashTable<LifetimeTracker> ht;

    CHECK(ht.empty());
    CHECK(ht.size() == 0);
    CHECK(ht.capacity() == 0);
  }

  SECTION("single item") {
    {
      HashTable<LifetimeTracker> ht;

      auto res = ht.insert(LifetimeTracker{10, 20});

      CHECK(res.is_new());
      CHECK(res.item().a() == 10);
      CHECK(res.item().b() == 20);

      CHECK(!ht.empty());
      CHECK(ht.size() == 1);
      CHECK(ht.capacity() != 0);

      CHECK(ht.contains(LifetimeTracker{10, 20}));
    }

    CHECK(LifetimeTracker::creates == 2);  // Constructed once as temporary passed into insert.
    CHECK(LifetimeTracker::copies == 0);
    CHECK(LifetimeTracker::destroys == 3);  // Temporary destroyed and item inside ht destroyed.
    CHECK(LifetimeTracker::moves == 1);     // Moved from temporary into storage.
  }

  SECTION("force rehash") {
    {
      HashTable<LifetimeTracker> ht;

      for (auto i = 0; i < 5; ++i) {
        // Make sure the values are unique so we get different hashes for each item.
        ht.insert(LifetimeTracker{10 + i, 20 + i});
      }

      MemSize total = 0;
      for (auto& item : ht) {
        total += item.a() + item.b();
      }
      CHECK(total == 170);

      CHECK(!ht.empty());
      CHECK(ht.size() == 5);
      CHECK(ht.capacity() > 4);  // Bigger then minimum size.
    }

    CHECK(LifetimeTracker::creates == 5);
    CHECK(LifetimeTracker::copies == 0);
    // CHECK(LifetimeTracker::destroys == 10);
    // CHECK(LifetimeTracker::moves == 15);
  }

  SECTION("remove") {
    HashTable<LifetimeTracker> ht;

    const I32 test_count = 100;
    for (I32 i = 0; i < test_count; ++i) {
      ht.insert({i, i});
    }

    for (I32 i = 0; i < test_count; ++i) {
      ht.contains({i, i});
    }

    for (I32 i = 0; i < test_count; ++i) {
      ht.remove({i, i});
      for (I32 j = i + 1; j < test_count; ++j) {
        CHECK(ht.contains({j, j}));
      }
    }
  }

  SECTION("basic") {
    HashTable<I32> t;

    auto res_1 = t.insert(10);
    CHECK(res_1.is_new());
    CHECK(res_1.item() == 10);

    auto res_2 = t.insert(10);
    CHECK(!res_2.is_new());
    CHECK(res_2.item() == 10);

    auto res_3 = t.insert(20);
    CHECK(res_3.is_new());
    CHECK(res_3.item() == 20);

    CHECK(t.contains(10));
    CHECK(!t.contains(11));

    CHECK(!t.remove(11));
    CHECK(t.remove(20));

    CHECK(!t.contains(20));

    CHECK(t.size() == 1);
  }

  SECTION("copy") {
    HashTable<I32> first;
    first.insert(10);
    first.insert(20);

    auto second = first;

    CHECK(first.size() == second.size());
    CHECK(first.capacity() == second.capacity());
  }
}

}  // namespace nu
