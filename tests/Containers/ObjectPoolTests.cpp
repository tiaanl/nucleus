#include "needle/Needle.h"
#include "nucleus/Containers/ObjectPool.h"

namespace nu {

struct Lifetime {
  static I32 constructed;
  static I32 destructed;

  I32 value;

  static auto reset() -> void {
    constructed = 0;
    destructed = 0;
  }

  explicit Lifetime(I32 value) : value{value} {
    constructed++;
  }

  ~Lifetime() {
    destructed++;
  }
};

// static
I32 Lifetime::constructed = 0;
I32 Lifetime::destructed = 0;

TEST(ObjectPoolTests, "basic construction") {
  ObjectPool<I32> pool;
  checkThat(pool.size()).isEqualTo(0);
  checkThat(pool.capacity()).isEqualTo(0);
  checkThat(pool.isEmpty()).isTrue();
}

TEST(ObjectPoolTests, "can acquire a new element") {
  ObjectPool<I32> pool;

  auto* element = pool.acquire(30);

  checkThat(pool.isEmpty()).isFalse();
  checkThat(element).isNotNull();
  checkThat(*element).isEqualTo(30);
}

TEST(ObjectPoolTests, "calls destructor on release") {
  Lifetime::reset();

  ObjectPool<Lifetime> pool;

  auto* element = pool.acquire(10);
  pool.release(element);

  checkThat(Lifetime::constructed).isEqualTo(1);
  checkThat(Lifetime::destructed).isEqualTo(1);
}

TEST(ObjectPoolTests, "destroyes elements on destruction") {
  Lifetime::reset();

  {
    ObjectPool<Lifetime> pool;

    auto* e1 = pool.acquire(10);
    auto* e2 = pool.acquire(20);

    checkThat(e1->value).isEqualTo(10);
    checkThat(e2->value).isEqualTo(20);

    checkThat(Lifetime::constructed).isEqualTo(2);
    checkThat(Lifetime::destructed).isEqualTo(0);
  }

  checkThat(Lifetime::destructed).isEqualTo(2);
}

}  // namespace nu
