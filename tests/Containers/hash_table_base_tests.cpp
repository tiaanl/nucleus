#include <catch2/catch.hpp>

#include "nucleus/Containers/hash_table_base.h"
#include "nucleus/testing/lifetime_tracker.h"

namespace nu {

using testing::LifetimeTracker;

class HashTableTest : public HashTableBase<LifetimeTracker> {
public:
  template <typename Predicate>
  Bucket* find_bucket_for_reading(HashedValue hash, Predicate predicate) const {
    return HashTableBase::find_bucket_for_reading(hash, predicate);
  }

  template <typename Predicate>
  Bucket* find_bucket_for_writing(HashedValue hash, Predicate predicate) {
    return HashTableBase::find_bucket_for_writing(hash, predicate);
  }
};

TEST_CASE("HashTableBase") {
  HashTableTest htb;

  SECTION("basic") {
    CHECK(htb.empty());
    CHECK(htb.size() == 0);
  }

  SECTION("find bucket on empty table") {
    HashedValue hash = 0;
    auto* bucket = htb.find_bucket_for_reading(hash, [](const LifetimeTracker&) {
      return false;
    });

    CHECK(bucket == nullptr);
  }

  SECTION("insert single item") {
    HashedValue hash = 0;
    auto* bucket = htb.find_bucket_for_writing(hash, [](const LifetimeTracker&) {
      return false;
    });

    CHECK(htb.capacity() > 0);

    CHECK(bucket != nullptr);
  }
}

}  // namespace nu
