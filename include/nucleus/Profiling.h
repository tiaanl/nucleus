#ifndef NUCLEUS_PROFILING_H_
#define NUCLEUS_PROFILING_H_

#include "nucleus/Containers/DynamicArray.h"
#include "nucleus/Memory/ScopedPtr.h"
#include "nucleus/Text/StaticString.h"
#include "nucleus/Text/StringView.h"

namespace nu {

namespace detail {

class ProfileMetrics {
public:
  struct Block {
    StaticString<128> name;

    Block* next;
    Block* prev;

    Block* parent;
    Block* children;

    F64 startTime;
    F64 stopTime;

    Block(const StringView& name, Block* prev, Block* parent, F64 startTime)
      : name{name},
        next{nullptr},
        prev{prev},
        parent{parent},
        children{nullptr},
        startTime{startTime},
        stopTime{0.0} {}
  };

  ProfileMetrics() noexcept;

  auto reset() -> void;

  auto startBlock(const StringView& name) -> void;
  auto stopBlock() -> void;

  auto root() -> Block* {
    return &m_root;
  }

private:
  Block m_root;
  Block* m_current;

  DynamicArray<Block*> m_blocks;
};

ProfileMetrics* getCurrentProfileMetrics();

}  // namespace detail

class Profiling {
public:
  Profiling();
  ~Profiling();

private:
  detail::ProfileMetrics* m_oldProfileMetrics;
  detail::ProfileMetrics m_profileMetrics;
};

class ScopedProfileBlock {
public:
  explicit ScopedProfileBlock(const StringView& name)
    : m_profiler(detail::getCurrentProfileMetrics()) {
    m_profiler->startBlock(name);
  }

  ~ScopedProfileBlock() {
    m_profiler->stopBlock();
  }

private:
  detail::ProfileMetrics* m_profiler;
};

#define PROFILE3(Description, Counter) ::nu::ScopedProfileBlock __PROFILE_##Counter(Description);
#define PROFILE2(Description, Counter) PROFILE3(Description, Counter)
#define PROFILE(Description) PROFILE2(Description, __COUNTER__)

}  // namespace nu

#endif  // NUCLEUS_PROFILING_H_
