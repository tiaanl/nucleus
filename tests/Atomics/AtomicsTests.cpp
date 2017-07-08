
#include "gtest/gtest.h"
#include "nucleus/Atomics/Atomics.h"

template <class AtomicType>
static void TestAtomicIncrement() {
    // For now, we just test single threaded execution.

    struct {
        AtomicType prev_word;
        AtomicType count;
        AtomicType next_word;
    } s;

    AtomicType prev_word_value, next_word_value;
    memset(&prev_word_value, 0xFF, sizeof(AtomicType));
    memset(&next_word_value, 0xEE, sizeof(AtomicType));

    s.prev_word = prev_word_value;
    s.count = 0;
    s.next_word = next_word_value;

    EXPECT_EQ(nu::detail::atomicIncrement(&s.count, 1), 1);
    EXPECT_EQ(s.count, 1);
    EXPECT_EQ(s.prev_word, prev_word_value);
    EXPECT_EQ(s.next_word, next_word_value);

    EXPECT_EQ(nu::detail::atomicIncrement(&s.count, 2), 3);
    EXPECT_EQ(s.count, 3);
    EXPECT_EQ(s.prev_word, prev_word_value);
    EXPECT_EQ(s.next_word, next_word_value);

    EXPECT_EQ(nu::detail::atomicIncrement(&s.count, 3), 6);
    EXPECT_EQ(s.count, 6);
    EXPECT_EQ(s.prev_word, prev_word_value);
    EXPECT_EQ(s.next_word, next_word_value);

    EXPECT_EQ(nu::detail::atomicIncrement(&s.count, -3), 3);
    EXPECT_EQ(s.count, 3);
    EXPECT_EQ(s.prev_word, prev_word_value);
    EXPECT_EQ(s.next_word, next_word_value);

    EXPECT_EQ(nu::detail::atomicIncrement(&s.count, -2), 1);
    EXPECT_EQ(s.count, 1);
    EXPECT_EQ(s.prev_word, prev_word_value);
    EXPECT_EQ(s.next_word, next_word_value);

    EXPECT_EQ(nu::detail::atomicIncrement(&s.count, -1), 0);
    EXPECT_EQ(s.count, 0);
    EXPECT_EQ(s.prev_word, prev_word_value);
    EXPECT_EQ(s.next_word, next_word_value);

    EXPECT_EQ(nu::detail::atomicIncrement(&s.count, -1), -1);
    EXPECT_EQ(s.count, -1);
    EXPECT_EQ(s.prev_word, prev_word_value);
    EXPECT_EQ(s.next_word, next_word_value);

    EXPECT_EQ(nu::detail::atomicIncrement(&s.count, -4), -5);
    EXPECT_EQ(s.count, -5);
    EXPECT_EQ(s.prev_word, prev_word_value);
    EXPECT_EQ(s.next_word, next_word_value);

    EXPECT_EQ(nu::detail::atomicIncrement(&s.count, 5), 0);
    EXPECT_EQ(s.count, 0);
    EXPECT_EQ(s.prev_word, prev_word_value);
    EXPECT_EQ(s.next_word, next_word_value);
}

#define NUM_BITS(T) (sizeof(T) * 8)

template <class AtomicType>
static void TestCompareAndSwap() {
    AtomicType value = 0;
    AtomicType prev = nu::detail::atomicCompareAndSwap(&value, 0, 1);
    EXPECT_EQ(1, value);
    EXPECT_EQ(0, prev);

    // Use test value that has non-zero bits in both halves, more for testing
    // 64-bit implementation on 32-bit platforms.
    const AtomicType k_test_val = (static_cast<uint64_t>(1) << (NUM_BITS(AtomicType) - 2)) + 11;
    value = k_test_val;
    prev = nu::detail::atomicCompareAndSwap(&value, 0, 5);
    EXPECT_EQ(k_test_val, value);
    EXPECT_EQ(k_test_val, prev);

    value = k_test_val;
    prev = nu::detail::atomicCompareAndSwap(&value, k_test_val, 5);
    EXPECT_EQ(5, value);
    EXPECT_EQ(k_test_val, prev);
}

template <class AtomicType>
static void TestAtomicExchange() {
    AtomicType value = 0;
    AtomicType new_value = nu::detail::atomicExchange(&value, 1);
    EXPECT_EQ(1, value);
    EXPECT_EQ(0, new_value);

    // Use test value that has non-zero bits in both halves, more for testing
    // 64-bit implementation on 32-bit platforms.
    const AtomicType k_test_val = (static_cast<uint64_t>(1) << (NUM_BITS(AtomicType) - 2)) + 11;
    value = k_test_val;
    new_value = nu::detail::atomicExchange(&value, k_test_val);
    EXPECT_EQ(k_test_val, value);
    EXPECT_EQ(k_test_val, new_value);

    value = k_test_val;
    new_value = nu::detail::atomicExchange(&value, 5);
    EXPECT_EQ(5, value);
    EXPECT_EQ(k_test_val, new_value);
}

template <class AtomicType>
static void TestAtomicIncrementBounds() {
    // Test at rollover boundary between int_max and int_min
    AtomicType test_val = (static_cast<uint64_t>(1) << (NUM_BITS(AtomicType) - 1));
    AtomicType value = -1 ^ test_val;
    AtomicType new_value = nu::detail::atomicIncrement(&value, 1);
    EXPECT_EQ(test_val, value);
    EXPECT_EQ(value, new_value);

    nu::detail::atomicIncrement(&value, -1);
    EXPECT_EQ(-1 ^ test_val, value);

    // Test at 32-bit boundary for 64-bit atomic type.
    test_val = static_cast<uint64_t>(1) << (NUM_BITS(AtomicType) / 2);
    value = test_val - 1;
    new_value = nu::detail::atomicIncrement(&value, 1);
    EXPECT_EQ(test_val, value);
    EXPECT_EQ(value, new_value);

    nu::detail::atomicIncrement(&value, -1);
    EXPECT_EQ(test_val - 1, value);
}

// Return an AtomicType with the value 0xa5a5a5..
template <class AtomicType>
static AtomicType TestFillValue() {
    AtomicType val = 0;
    memset(&val, 0xa5, sizeof(AtomicType));
    return val;
}

// This is a simple sanity check that values are correct. Not testing
// atomicity
template <class AtomicType>
static void TestStore() {
    const AtomicType kVal1 = TestFillValue<AtomicType>();
    const AtomicType kVal2 = static_cast<AtomicType>(-1);

    AtomicType value;

    nu::detail::atomicStore(&value, kVal1);
    EXPECT_EQ(kVal1, value);
    nu::detail::atomicStore(&value, kVal2);
    EXPECT_EQ(kVal2, value);
}

// This is a simple sanity check that values are correct. Not testing
// atomicity
template <class AtomicType>
static void TestLoad() {
    const AtomicType kVal1 = TestFillValue<AtomicType>();
    const AtomicType kVal2 = static_cast<AtomicType>(-1);

    AtomicType value;

    value = kVal1;
    EXPECT_EQ(kVal1, nu::detail::atomicLoad(&value));
    value = kVal2;
    EXPECT_EQ(kVal2, nu::detail::atomicLoad(&value));
}

TEST(AtomicOpsTest, Inc) {
    TestAtomicIncrement<nu::detail::Atomic32>();
}

TEST(AtomicOpsTest, CompareAndSwap) {
    TestCompareAndSwap<nu::detail::Atomic32>();
}

TEST(AtomicOpsTest, Exchange) {
    TestAtomicExchange<nu::detail::Atomic32>();
}

TEST(AtomicOpsTest, IncrementBounds) {
    TestAtomicIncrementBounds<nu::detail::Atomic32>();
}

TEST(AtomicOpsTest, Store) {
    TestStore<nu::detail::Atomic32>();
}

TEST(AtomicOpsTest, Load) {
    TestLoad<nu::detail::Atomic32>();
}
