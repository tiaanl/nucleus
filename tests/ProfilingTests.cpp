#include "needle/Needle.h"

#include "nucleus/Profiling.h"

namespace nu {

TEST(ProfilingTests, "by default no profiling metrics exist") {
  // This should not really be a test?
  checkThat(detail::getCurrentProfileMetrics()).isNull();
}

TEST(ProfilingTests, "profiling metrics are linked to a Profiling object on the stack") {
  {
    Profiling profiling;

    checkThat(detail::getCurrentProfileMetrics()).isNotNull();
  }

  checkThat(detail::getCurrentProfileMetrics()).isNull();
}

TEST(ProfilingTests, "can add a first block") {
  Profiling profiling;

  PROFILE("first")

  auto metrics = detail::getCurrentProfileMetrics();

  checkThat(metrics->root()->children).isNotNull();
  checkThat(metrics->root()->next).isNull();
  checkThat(metrics->root()->children->name).isEqualTo("first");
}

TEST(ProfilingTests, "add second node as child") {
  Profiling profiling;

  PROFILE("first")
  PROFILE("second")

  auto metrics = detail::getCurrentProfileMetrics();

  checkThat(metrics->root()->children).isNotNull();
  checkThat(metrics->root()->children->name).isEqualTo("first");
  checkThat(metrics->root()->children->children).isNotNull();
  checkThat(metrics->root()->children->children->name).isEqualTo("second");
}

}  // namespace nu
