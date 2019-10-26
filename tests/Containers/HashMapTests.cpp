#include "needle/Needle.h"
#include "nucleus/Containers/HashMap.h"
#include "nucleus/Text/StaticString.h"

namespace nu {

TEST(HashMapTests, "basic") {
  HashMap<MemSize, StaticString<64>> hm;

  checkThat(hm.size()).isEqualTo(0);
}

}  // namespace nu