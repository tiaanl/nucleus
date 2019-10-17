#ifndef NUCLEUS_PROFILING_H_
#define NUCLEUS_PROFILING_H_

#include "nucleus/Text/StaticString.h"
#include "nucleus/Text/StringView.h"

namespace nu {

class Profiler {
public:
  struct Block {
    nu::StaticString<128> name;

    Block* next;
    Block* prev;

    Block* parent;
    Block* children;
  };

  Profiler() noexcept;

  auto reset() -> void;

  auto startBlock(const StringView& name) -> void;
  auto stopBlock() -> void;

  auto root() -> Block* {
    return &m_root;
  }

private:
  Block m_root;
  Block* m_current;
};

Profiler* getGlobalProfiler();

class ScopedProfileBlock {
public:
  ScopedProfileBlock(const StringView& name) : m_profiler(getGlobalProfiler()) {
    m_profiler->startBlock(name);
  }

  ~ScopedProfileBlock() {
    m_profiler->stopBlock();
  }

private:
  Profiler* m_profiler;
};

#define PROFILE3(Description, Counter) ::nu::ScopedProfileBlock __PROFILE_##Counter(Description);
#define PROFILE2(Description, Counter) PROFILE3(Description, Counter)
#define PROFILE(Description) PROFILE2(Description, __COUNTER__)

}  // namespace nu

#endif  // NUCLEUS_PROFILING_H_
