
#ifndef NUCLEUS_SYNCHRONIZATION_AUTO_LOCK_H_
#define NUCLEUS_SYNCHRONIZATION_AUTO_LOCK_H_

template <typename T>
class AutoLock {
public:
  AutoLock(T& lock) : m_lock(lock) { m_lock.Acquire(); }

  ~AutoLock() { m_lock.Release(); }

private:
  T& m_lock;
};

#endif  // NUCLEUS_SYNCHRONIZATION_AUTO_LOCK_H_
