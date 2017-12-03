
#include "nucleus/strings/char_ptr.h"

#include "gtest/gtest.h"

#include "nucleus/MemoryDebug.h"

namespace nu {

TEST(CharPtrTest, Basic) {
  const char kTestString[] = "testing";
  CharPtrA a((kTestString));
}

}  // namespace nu
