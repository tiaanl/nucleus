
#include "nucleus/Singleton.h"
#include "nucleus/Testing.h"
#include "nucleus/Types.h"

namespace nu {

class First {
public:
  static First& getInstance() {
    return Singleton<First>::get();
  }

  First() : m_count(1) {
    int a = 10;
  }

  ~First() {
    int a = 20;
  }

  I32 getCount() const {
    return m_count;
  }

  void increment() {
    ++m_count;
  }

private:
  I32 m_count;
};

TEST_CASE("Singleton") {
  First::getInstance().increment();
  CHECK(First::getInstance().getCount() == 2);
}

}  // namespace nu
