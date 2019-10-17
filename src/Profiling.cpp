#include "nucleus/Profiling.h"

namespace nu {

namespace {

Profiler g_globalProfiler;

}  // namespace

Profiler::Profiler() noexcept
  : m_root{nu::StringView{"root"}, nullptr, nullptr, nullptr, nullptr}, m_current{&m_root} {}

auto Profiler::reset() -> void {
  m_current = &m_root;
  m_root.next = nullptr;
  m_root.children = nullptr;
  return;

  Block* last = &m_root;
  while (last->next) {
    last = last->next;
  }

  if (last == &m_root) {
    return;
  }

  auto current = last;
  while (true) {
    auto prev = current->prev;

    // This might be nullptr, but that's OK.
    delete current;
    current = prev;

    if (prev == &m_root) {
      break;
    }
  }

  m_current = &m_root;
}

auto Profiler::startBlock(const StringView& name) -> void {
  if (m_current->children) {
    // Find the last child.
    auto c = m_current->children;
    while (c->next) {
      c = c->next;
    }

    auto newBlock = new Block{name, nullptr, c, m_current, nullptr};
    c->next = newBlock;

    m_current = newBlock;
  } else {
    auto newBlock = new Block{name, nullptr, nullptr, m_current, nullptr};
    m_current->children = newBlock;
    m_current = newBlock;
  }
}

auto Profiler::stopBlock() -> void {
  if (m_current->prev) {
    m_current = m_current->prev;
  } else {
    m_current = m_current->parent;
  }
}

Profiler* getGlobalProfiler() {
  return &g_globalProfiler;
}

}  // namespace nu
