#pragma once

template <typename T>
class AutoLock {
public:
  AutoLock(T& lock) : m_lock(lock) {
    m_lock.acquire();
  }

  ~AutoLock() {
    m_lock.release();
  }

private:
  T& m_lock;
};
