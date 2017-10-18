
#ifndef NUCLEUS_MEMORY_SCOPED_PTR_H_
#define NUCLEUS_MEMORY_SCOPED_PTR_H_

#include "nucleus/utils/CommonType.h"
#include "nucleus/utils/Functional.h"

namespace nu {

namespace detail {

template <typename T>
struct DefaultDelete {
  void operator()(T* ptr) const { delete ptr; }
};

template <typename T>
struct DefaultDelete<T[]> {
  void operator()(T* ptr) const { delete[] ptr; }
};

}  // namespace detail

template <typename T, typename Deleter = detail::DefaultDelete<T>>
class ScopedPtr {
public:
  using ElementType = T;
  using DeleterType = Deleter;

  // Construct an empty ScopedPtr that owns nothing.
  constexpr ScopedPtr() : m_ptr(nullptr) {}

  // Construct a new ScopedPtr and take ownership of the |ptr| that is passed in.
  ScopedPtr(ElementType* ptr) : m_ptr(ptr) {}

  // Construct a new ScopedPtr and take ownership of the |ptr| that is passed in.  Also set the deleter that should be
  // used when deleting the owned pointer.
  ScopedPtr(ElementType* ptr, DeleterType deleter) : m_ptr(ptr), m_deleter(deleter) {}

  // We do not allow copying of a ScopedPtr.
  ScopedPtr(const ScopedPtr&) = delete;

  // Construct a new ScopedPtr by taking ownership of the pointer that the passed in ScopedPtr might own.
  ScopedPtr(ScopedPtr&& other) : m_ptr(other.release()), m_deleter(forward<DeleterType>(other.getDeleter())) {}

  template <typename T2, typename D2>
  ScopedPtr(ScopedPtr<T2, D2>&& other) : m_ptr(other.release()), m_deleter(forward<D2>(other.getDeleter())) {}

  // Destruct the ScopedPtr and delete the pointer we might own.
  ~ScopedPtr() { reset(); }

  // We do not allow copying of ScopedPtr's.
  ScopedPtr& operator=(const ScopedPtr&) = delete;

  ScopedPtr& operator=(ScopedPtr&& x) {
    reset(x.release());
    m_deleter = move(forward<DeleterType>(x.m_deleter));
    return *this;
  }

  template <typename T2, typename D2>
  ScopedPtr& operator=(ScopedPtr<T2, D2>&& other) {
    reset(other.release());
    m_deleter = move(forward<D2>(other.m_deleter));
    return *this;
  };

  // Dereference the ScopedPtr to get a reference to the original type.
  ElementType* operator->() const { return m_ptr; }
  ElementType& operator*() const { return *m_ptr; }

  // Return the pointer we own.
  ElementType* get() const { return m_ptr; }

  // Return the deleter that we use to delete the owned pointer.
  DeleterType getDeleter() const { return m_deleter; }

  // Release the pointer we own to the caller.
  ElementType* release() {
    ElementType* const tmp = m_ptr;
    m_ptr = nullptr;
    return tmp;
  }

  void reset(ElementType* ptr = nullptr) {
    if (ptr != m_ptr) {
      m_deleter(m_ptr);
      m_ptr = ptr;
    }
  }

  void swap(ScopedPtr& other) {
    ElementType* tmp = m_ptr;
    m_ptr = other.m_ptr;
    other.m_ptr = tmp;
  }

  explicit operator bool() const { return m_ptr != nullptr; }

protected:
  ElementType* m_ptr;
  DeleterType m_deleter;
};

template <typename T, typename D>
inline void swap(ScopedPtr<T, D>& left, ScopedPtr<T, D>& right) {
  left.swap(right);
};

template <typename T1, typename D1, typename T2, typename D2>
inline bool operator<(const ScopedPtr<T1, D1>& left, const ScopedPtr<T2, D2>& right) {
  using P1 = typename ScopedPtr<T1, D1>::ElementType*;
  using P2 = typename ScopedPtr<T2, D2>::ElementType*;
  using Common = CommonType<P1, P2>;

  Common t1 = left.get();
  Common t2 = right.get();

  return Less<Common>()(t1, t2);
};

template <typename T, typename... Args>
inline ScopedPtr<T> MakeScopedPtr(Args&&... args) {
  return ScopedPtr<T>(new T(forward<Args>(args)...));
};

}  // namespace nu

#endif  // NUCLEUS_MEMORY_SCOPED_PTR_H_
