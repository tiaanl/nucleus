#include "nucleus/Profiling.h"

#include "nucleus/HighPerformanceTimer.h"
#include "nucleus/Logging.h"

namespace nu {

namespace detail {

namespace {

ProfileMetrics* g_globalProfileMetrics = nullptr;

}  // namespace

ProfileMetrics::ProfileMetrics() noexcept
  : m_root{nu::StringView{"root"}, nullptr, nullptr, 0.0f}, m_current{&m_root} {}

auto ProfileMetrics::reset() -> void {
  m_blocks.releaseAll();

  m_current = &m_root;
  m_root.children = nullptr;
}

auto ProfileMetrics::startBlock(const StringView& name) -> void {
  if (!g_globalProfileMetrics) {
    LOG(Fatal) << "No profile metrics defined. Create a Profiling object on the stack.";
    return;
  }

  if (m_current->children) {
    // Find the last child.
    auto lastChild = m_current->children;
    while (lastChild->next) {
      lastChild = lastChild->next;
    }

    auto newBlock = m_blocks.acquire(name, lastChild, nullptr, nu::getCurrentHighPerformanceTick());

    lastChild->next = newBlock;

    m_current = newBlock;
  } else {
    auto newBlock = m_blocks.acquire(name, nullptr, m_current, nu::getCurrentHighPerformanceTick());

    m_current->children = newBlock;
    m_current = newBlock;
  }
}

auto ProfileMetrics::stopBlock() -> void {
  m_current->stopTime = nu::getCurrentHighPerformanceTick();

  if (m_current->prev) {
    m_current = m_current->prev;
  } else if (m_current->parent) {
    m_current = m_current->parent;
  }
}

ProfileMetrics* getCurrentProfileMetrics() {
  return g_globalProfileMetrics;
}

}  // namespace detail

Profiling::Profiling() : m_oldProfileMetrics{detail::g_globalProfileMetrics} {
  detail::g_globalProfileMetrics = &m_profileMetrics;
}

Profiling::~Profiling() {
  detail::g_globalProfileMetrics = m_oldProfileMetrics;
}

}  // namespace nu