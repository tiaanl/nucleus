
#include "nucleus/AtExit.h"
#include "nucleus/Testing.h"
#include "nucleus/Types.h"

namespace nu {

I32 g_testCounter1 = 0;
I32 g_testCounter2 = 0;

void incrementTestCounter1(void*) {
  ++g_testCounter1;
}

void incrementTestCounter2(void*) {
  ++g_testCounter2;
}

void resetTestCounters() {
  g_testCounter1 = 0;
  g_testCounter2 = 0;
}

void expectCounter1IsZero(void*) {
  CHECK(g_testCounter1 == 0);
}

void expectParamIsNull(void* param) {
  CHECK(param == nullptr);
}

void expectParamIsCounter(void* param) {
  CHECK(&g_testCounter1 == param);
}

TEST_CASE("AtExit") {
  AtExit atExit;

  SECTION("basic") {
    resetTestCounters();
    AtExit::registerCallback(&incrementTestCounter1, nullptr);
    AtExit::registerCallback(&incrementTestCounter2, nullptr);
    AtExit::registerCallback(&incrementTestCounter1, nullptr);

    CHECK(g_testCounter1 == 0);
    CHECK(g_testCounter2 == 0);
    AtExit::processCallbacksNow();
    CHECK(g_testCounter1 == 2);
    CHECK(g_testCounter2 == 1);
  }

  SECTION("LIFO order") {
    resetTestCounters();
    AtExit::registerCallback(&incrementTestCounter1, nullptr);
    AtExit::registerCallback(&expectCounter1IsZero, nullptr);
    AtExit::registerCallback(&incrementTestCounter2, nullptr);

    CHECK(g_testCounter1 == 0);
    CHECK(g_testCounter2 == 0);
    AtExit::processCallbacksNow();
    CHECK(g_testCounter1 == 1);
    CHECK(g_testCounter2 == 1);
  }

  SECTION("Params") {
    AtExit::registerCallback(&expectParamIsNull, nullptr);
    AtExit::registerCallback(&expectParamIsCounter, &g_testCounter1);
    AtExit::processCallbacksNow();
  }
}

}  // namespace nu
