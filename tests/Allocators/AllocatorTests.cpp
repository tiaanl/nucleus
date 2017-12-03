
#include "gtest/gtest.h"

#if 0
#include "nucleus/Memory/Allocator.h"
#include "nucleus/config.h"

TEST(AllocatorTests, BasicTest) {
    const USize sizeToAllocate = 100;
    const char* kTestString = "This is a test";

    nu::Allocator allocator;
    char* memory = static_cast<char*>(allocator.allocate(sizeToAllocate));

#if COMPILER(MSVC)
    strcpy_s(memory, sizeToAllocate, kTestString);
#else
    strcpy(memory, kTestString);
#endif

    ASSERT_STREQ(kTestString, memory);

    allocator.free(memory, 100);
}

TEST(AllocatorTests, Name) {
    const char* kTestAllocatorName = "MyAllocator";
    const char* anotherAllocatorName = "AnotherAllocator";

    nu::Allocator allocator(kTestAllocatorName);
    ASSERT_STREQ(kTestAllocatorName, allocator.getName());

    allocator.setName("AnotherAllocator");
    ASSERT_STREQ(anotherAllocatorName, allocator.getName());
}
#endif  // 0
